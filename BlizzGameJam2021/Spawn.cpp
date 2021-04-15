#include "Spawn.h"
#include "Constants.h"
#include "Game.h"
#include "Map.h"
#include "MapTile.h"
#include "Display.h"

#if _DEBUG
	#include <assert.h>
#endif

#pragma region Constructor

Spawn::Spawn(int id, double spawnX, double spawnY, int width, int height, const std::string& texturePath, int spriteSheetOffsetX, int spriteSheetOffsetY, bool shouldIdleMove)
	: Object(spawnX, spawnY, width, height, texturePath, RenderLayers::SPAWNS), id(id)
{
	this->spriteSheetOffsetX = spriteSheetOffsetX;
	this->spriteSheetOffsetY = spriteSheetOffsetY;
	this->shouldIdleMove = shouldIdleMove;
	this->idleMoveCooldown = 0;
	this->idleDirection = Direction::NONE;
}

#pragma endregion

#pragma region Public Methods

Spawn::~Spawn()
{
}

void Spawn::InjectFrame(unsigned int elapsedGameTime, unsigned int previousFrameTime)
{
	double startPosX = this->x;
	double startPosY = this->y;
	int startTileRow = static_cast<int>((this->y - (this->height / 2)) / TILE_HEIGHT);
	int startTileColumn = static_cast<int>((this->x - (this->width / 2)) / TILE_WIDTH);

	bool didMoveThisFrame = false;

	if (this->shouldIdleMove)
	{
		if (this->idleMoveCooldown <= 0)
		{
			/* generate secret number between 0 and 4 (0 = no move, and 1-4 for each axis: */
			int direction = rand() % 5;

			switch (direction)
			{
			case 0: this->idleDirection = Direction::NONE;	break;
			case 1:	this->idleDirection = Direction::UP;	break;
			case 2:	this->idleDirection = Direction::DOWN;	break;
			case 3:	this->idleDirection = Direction::LEFT;	break;
			case 4:	this->idleDirection = Direction::RIGHT;	break;
			default:
#if _DEBUG
				assert(false);	//we should never hit here!
#endif
				break;
			}

			//reset idle movement cooldown
			this->idleMoveCooldown = NPC_IDLEMOVEMENT_COOLDOWN;
		}
		else
		{
			float previousFrameTimeInSeconds = (previousFrameTime / 1000.0f);

			switch (this->idleDirection)
			{
			case Direction::NONE:
				//do nothing
				break;
			case Direction::UP:
				this->y -= NPC_VELOCITY * previousFrameTimeInSeconds;
				didMoveThisFrame = true;
				break;
			case Direction::DOWN:
				this->y += NPC_VELOCITY * previousFrameTimeInSeconds;
				didMoveThisFrame = true;
				break;
			case Direction::LEFT:
				this->x -= NPC_VELOCITY * previousFrameTimeInSeconds;
				didMoveThisFrame = true;
				break;
			case Direction::RIGHT:
				this->x += NPC_VELOCITY * previousFrameTimeInSeconds;
				didMoveThisFrame = true;
				break;
			default:
#if _DEBUG
				assert(false);	//we should never hit here!
#endif
				break;
			}

			this->idleMoveCooldown -= previousFrameTime;
		}
	}

	if (didMoveThisFrame)
	{
		//enforce screen bounds
		int halfWidth = this->width / 2;
		int halfHeight = this->height / 2;

		const Game* game = Game::GetInstance();
		const Map* map = game->GetMap();
#if _DEBUG
	assert(map);
#endif
		const int mapWidth = map->GetColumnCount() * TILE_WIDTH;
		const int mapHeight = map->GetRowCount() * TILE_HEIGHT;

		if (this->x - halfWidth < 0)
		{
			this->x = halfWidth;
		}
		else if (this->x + halfWidth > mapWidth)
		{
			this->x = mapWidth - halfWidth;
		}

		if (this->y - halfHeight < 0)
		{
			this->y = halfHeight;
		}
		else if (this->y + halfHeight > mapHeight)
		{
			this->y = mapHeight - halfHeight;
		}

		//check if we're attempting to cross to a new tile that isn't walkable
		int endTileRow = static_cast<int>((this->y - (this->height / 2)) / TILE_HEIGHT);
		int endTileColumn = static_cast<int>((this->x - (this->width / 2)) / TILE_WIDTH);

		if (startTileRow != endTileRow || startTileColumn != endTileColumn)
		{
			//we crossed into a new tile, check if it's walkable
			const int numberOfMapLayers = map->GetNumberOfLayers();
			for (int layer = 0; layer < numberOfMapLayers; layer++)
			{
				//each layer of the map has different walkable data so have to check each layer
				const MapTile* tile = map->GetTileByWorldGridLocation(endTileRow, endTileColumn, layer);
				if (tile == nullptr || !tile->GetIsWalkable())
				{
					//not walkable, so move them back!
					this->x = startPosX;
					this->y = startPosY;
					break;
				}
			}
		}
	}
}

void Spawn::Draw()
{ 
	const Game* game = Game::GetInstance();
	const SDL_Rect& camera = game->GetCamera();

	Display::QueueTextureForRendering(this->texture, this->x - camera.x, this->y - camera.y, this->width, this->height, true, this->layer, true, this->spriteSheetOffsetX, this->spriteSheetOffsetY);
}

int Spawn::GetID()
{
	return this->id;
}

#pragma endregion
