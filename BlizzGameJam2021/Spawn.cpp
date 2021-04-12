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

Spawn::Spawn(int id, double spawnX, double spawnY, int width, int height, const std::string& texturePath, int spriteSheetOffsetX, int spriteSheetOffsetY)
	: Object(spawnX, spawnY, width, height, texturePath), id(id)
{
	this->spriteSheetOffsetX = spriteSheetOffsetX;
	this->spriteSheetOffsetY = spriteSheetOffsetY;
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

	Display::QueueTextureForRendering(this->texture, this->x - camera.x, this->y - camera.y, this->width, this->height, true, true, this->spriteSheetOffsetX, this->spriteSheetOffsetY);
}

int Spawn::GetID()
{
	return this->id;
}

#pragma endregion
