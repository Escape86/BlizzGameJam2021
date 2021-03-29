#include "Game.h"
#include "Player.h"
#include "Map.h"
#include "Constants.h"
#include "SDL_timer.h"
#include "SDL_keycode.h"

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
	this->player->SetPosition(96, 888);

	//init camera
	this->camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//load initial map
	this->map = new Map("../resources/western.csv", "../resources/western.png");

	Game::_instance = this;
}

#pragma endregion

#pragma region Public Methods

Game::~Game()
{
	if (this->player)
	{
		delete this->player;
		this->player = nullptr;
	}

	if (this->map)
	{
		delete this->map;
		this->map = nullptr;
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

	//update player
	this->player->InjectFrame(elapsedTimeInMilliseconds, previousFrameTime);
	
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

#pragma endregion