#include "Game.h"
#include "Player.h"
#include "Map.h"
#include "Teleporter.h"
#include "Constants.h"
#include "SDL_timer.h"
#include "SDL_keycode.h"
#include <fstream>

#if _DEBUG
	#include <assert.h>
#endif

Game* Game::_instance;

#pragma region Constructor

Game::Game()
	: previousFrameEndTime(0)
{
#if _DEBUG
	assert(Game::_instance == nullptr);	//already initialized!
#endif	

	//init player
	this->player = new Player(0.0, 0.0, Direction::DOWN);
	this->player->SetPosition(PLAYER_SPAWN_POSITION_X, PLAYER_SPAWN_POSITION_Y);

	//init camera
	this->camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//load initial map
	std::vector<std::string> mapDataFilePaths = { STARTING_HOUSE_MAP_DATA_FILEPATH0, STARTING_HOUSE_MAP_DATA_FILEPATH1, STARTING_HOUSE_MAP_DATA_FILEPATH2 };
	this->SwitchMap(mapDataFilePaths, INTERIOR_TILESET_TEXTURE_FILEPATH, STARTING_HOUSE_MAP_TELEPORTERS_FILEPATH);

	Game::_instance = this;
}

#pragma endregion

#pragma region Public Methods

Game::~Game()
{
	this->cleanUpGameObjects();

	if (this->player)
	{
		delete this->player;
		this->player = nullptr;
	}
}

const Game* Game::GetInstance()
{
	return Game::_instance;
}

void Game::InjectFrame()
{
#if _DEBUG
	assert(this->player);
	assert(this->map);
#endif

	Uint32 elapsedTimeInMilliseconds = SDL_GetTicks();

	const unsigned int previousFrameTime = elapsedTimeInMilliseconds - this->previousFrameEndTime;

	float frameTimeInMilliseconds = previousFrameTime / 1000.0f;

	if (this->mapSwitchRequested)
	{
		const Destination& destination = this->destinationMapSwitch;
		if (this->SwitchMap(destination.destinationMapFilePathsByLayer, destination.destinationMapTextureFilePath, destination.destinationTeleportersFilePath))
		{
			//move player to requested location in new map
			this->player->SetPosition(destination.destinationX, destination.destinationY);

			this->mapSwitchRequested = false;
		}

		return;
	}

	//update player
	this->player->InjectFrame(elapsedTimeInMilliseconds, previousFrameTime);
	
	//check if in teleporter
	for (const Teleporter& tp : this->teleporters)
	{
		if (tp.TestCollision(this->player))
		{
			this->destinationMapSwitch = tp.GetDestination();
			this->mapSwitchRequested = true;
		}
	}

	//center the camera over the player
	camera.x = (this->player->GetPositionX() + PLAYER_WIDTH / 2) - SCREEN_WIDTH / (2 * RENDER_SCALE_AMOUNT);
	camera.y = (this->player->GetPositionY() + PLAYER_HEIGHT / 2) - SCREEN_HEIGHT / (2 * RENDER_SCALE_AMOUNT);

	//Keep the camera in bounds
	const int mapWidth = this->map->GetColumnCount() * TILE_WIDTH;
	const int mapHeight = this->map->GetRowCount() * TILE_HEIGHT;
	if (camera.x < 0)
	{
		camera.x = 0;
	}
	else if ((camera.x + (camera.w / 2)) > (mapWidth - (TILE_WIDTH / 2)))
	{
		camera.x = mapWidth - (camera.w / 2) - (TILE_WIDTH / 2);
	}
	if (camera.y < 0)
	{
		camera.y = 0;
	}
	else if ((camera.y + (camera.h / 2)) > (mapHeight - (TILE_HEIGHT / 2)))
	{
		camera.y = mapHeight - (camera.h / 2) - (TILE_HEIGHT / 2);
	}

	//now that updates are done, draw the frame
	this->map->Draw(camera.x, camera.y);
	this->player->Draw();

	//end of frame
	this->previousFrameEndTime = elapsedTimeInMilliseconds;
}

void Game::InjectKeyDown(int key)
{
#if _DEBUG
	assert(player);
#endif

	this->player->OnKeyDown(key);
}

void Game::InjectKeyUp(int key)
{
#if _DEBUG
	assert(player);
#endif

	this->player->OnKeyUp(key);
}

void Game::InjectControllerStickMovement(unsigned char axis, short value)
{
	//X axis motion
	if (axis == 0)
	{
		//Left of dead zone
		if (value < -JOYSTICK_DEAD_ZONE)
		{
			//fake a keypress for the left direction
			this->player->OnKeyDown(SDLK_LEFT);
		}
		//Right of dead zone
		else if (value > JOYSTICK_DEAD_ZONE)
		{
			//fake a keypress for the right direction
			this->player->OnKeyDown(SDLK_RIGHT);
		}
		//In horizontal dead zone
		else
		{
			this->player->ResetHorizontalVelocity();
		}
	}
	//Y axis motion
	else if (axis == 1)
	{
		//Above dead zone
		if (value < -JOYSTICK_DEAD_ZONE)
		{
			//fake a keypress for the up direction
			this->player->OnKeyDown(SDLK_UP);
		}
		//Below dead zone
		else if (value > JOYSTICK_DEAD_ZONE)
		{
			//fake a keypress for the down direction
			this->player->OnKeyDown(SDLK_DOWN);
		}
		//In vertical dead zone
		else
		{
			this->player->ResetVerticalVelocity();
		}
	}
}

bool Game::SwitchMap(const std::vector<std::string>& mapFilePathsByLayer, const std::string& mapTextureFilePath, const std::string& teleportersFilePath)
{
	//nuke any existing map stuff we have loaded so we can make a fresh start (and not leak memory)
	this->cleanUpGameObjects();

	this->map = new Map(mapFilePathsByLayer, mapTextureFilePath);

	if (!map)
		return false;

	bool loadTeleportersResult = this->loadTeleporters(teleportersFilePath);

#if _DEBUG
	assert(loadTeleportersResult);
#endif

	if (!loadTeleportersResult)
		return false;

	return true;
}

const Player* Game::GetPlayer() const
{
	return this->player;
}

const Map* Game::GetMap() const
{
	return this->map;
}

const SDL_Rect& Game::GetCamera() const
{
	return this->camera;
}

#pragma endregion

#pragma region Private Methods

void Game::cleanUpGameObjects()
{
	this->teleporters.clear();

	if (this->map)
	{
		delete this->map;
		this->map = nullptr;
	}
}

bool Game::loadTeleporters(const std::string& filepath)
{
	std::ifstream file(filepath.c_str());

	if (!file.is_open())
		return false;

	std::string line;
	while (std::getline(file, line))
	{
		if (line.length() < 2)
			continue;

		//skip comment lines
		if (line[0] == '-' && line[1] == '-')
			continue;

		char* l = _strdup(line.c_str());

		char* context = nullptr;

		char* xPosToken = strtok_s(l, ";", &context);
		char* yPosToken = strtok_s(nullptr, ";", &context);
		char* destinationMapFilePathToken = strtok_s(nullptr, ";", &context);
		char* destinationMapTextureFilePathToken = strtok_s(nullptr, ";", &context);
		char* destinationTeleportersFilePathToken = strtok_s(nullptr, ";", &context);
		char* destinationXToken = strtok_s(nullptr, ";", &context);
		char* destinationYToken = strtok_s(nullptr, ";", &context);

		if ((xPosToken == nullptr) ||
			(yPosToken == nullptr) ||
			(destinationMapFilePathToken == nullptr) ||
			(destinationMapTextureFilePathToken == nullptr) ||
			(destinationTeleportersFilePathToken == nullptr) ||
			(destinationXToken == nullptr) ||
			(destinationYToken == nullptr))
			return false;

		int xPos = atoi(xPosToken);
		int yPos = atof(yPosToken);
		std::string destinationMapFilePathsAsString = destinationMapFilePathToken;
		std::string destinationMapTextureFilePath = destinationMapTextureFilePathToken;
		std::string destinationTeleportersFilePath = destinationTeleportersFilePathToken;
		int destinationX = atoi(destinationXToken);
		int destinationY = atoi(destinationYToken);

		//clear whitespace from destinationMapFilePathAsString
		while (destinationMapFilePathsAsString.size() && isspace(destinationMapFilePathsAsString.front()))	//front
			destinationMapFilePathsAsString.erase(destinationMapFilePathsAsString.begin());
		while (destinationMapFilePathsAsString.size() && isspace(destinationMapFilePathsAsString.back()))	//back
			destinationMapFilePathsAsString.pop_back();

		//clear whitespace from destinationMapTextureFilePath
		while (destinationMapTextureFilePath.size() && isspace(destinationMapTextureFilePath.front()))	//front
			destinationMapTextureFilePath.erase(destinationMapTextureFilePath.begin());
		while (destinationMapTextureFilePath.size() && isspace(destinationMapTextureFilePath.back()))	//back
			destinationMapTextureFilePath.pop_back();

		//clear whitespace from destinationTeleportersFilePath
		while (destinationTeleportersFilePath.size() && isspace(destinationTeleportersFilePath.front()))	//front
			destinationTeleportersFilePath.erase(destinationTeleportersFilePath.begin());
		while (destinationTeleportersFilePath.size() && isspace(destinationTeleportersFilePath.back()))	//back
			destinationTeleportersFilePath.pop_back();

		//split destinationMapFilePathsAsString in individual strings
		std::vector<std::string> destinationMapFilePaths;

		size_t startPosition = 0;
		size_t endPosition = destinationMapFilePathsAsString.find(',', startPosition);
		while (endPosition != std::string::npos)
		{
			destinationMapFilePaths.push_back(destinationMapFilePathsAsString.substr(startPosition, endPosition - startPosition));

			startPosition = endPosition + 1; //+1 to skip delimiter
			endPosition = destinationMapFilePathsAsString.find(',', startPosition);
		}

		//double-check we've got one left to add
#if _DEBUG
		assert(startPosition != 0 && startPosition != std::string::npos);
#endif

		//no delimiter after last one, so we're done, but don't forget to include last
		destinationMapFilePaths.push_back(destinationMapFilePathsAsString.substr(startPosition));

		//clear whitespace from destinationMapFilePaths
		for (std::string& fp : destinationMapFilePaths)
		{
			while (fp.size() && isspace(fp.front()))	//front
				fp.erase(fp.begin());
			while (fp.size() && isspace(fp.back()))	//back
				fp.pop_back();
		}

		//all done, build our teleporter and add it to the set
		this->teleporters.emplace_back(xPos, yPos, TELEPORTER_WIDTH, TELEPORTER_HEIGHT, destinationMapFilePaths, destinationMapTextureFilePath, destinationTeleportersFilePath, destinationX, destinationY);

		free(l);
	}

	file.close();

	return true;
}

#pragma endregion