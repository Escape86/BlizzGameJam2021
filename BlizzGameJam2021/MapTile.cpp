#include "MapTile.h"
#include "Display.h"
#include "Constants.h"
#include <map>

#if _DEBUG
	#include <assert.h>
#endif

struct TileInfo
{
	int spriteSheetRowOffset;
	int spriteSheetColumnOffset;
	bool walkAble;
};

std::map<int, TileInfo> westernTileIdToInfoLookup		// { id, { spriteSheetRowOffset, spriteSheetColumnOffset, walkAble }}
{
	{ 0,  { 0,  0, false } },
	{ 1,  { 0,  1, true } },
	{ 2,  { 0,  2, true } },
	{ 3,  { 1,  0, true } },
	{ 4,  { 1,  1, false } },
	{ 5,  { 1,  2, false } },
	{ 6,  { 2,  0, false } },
	{ 7,  { 2,  1, true } },
	{ 8,  { 2,  2, true } },
	{ 9,  { 3,  0, false } },
	{ 10, { 3,  1, false } },
	{ 11, { 3,  2, false } },
	{ 12, { 4,  0, false } },
	{ 13, { 4,  1, false } },
	{ 14, { 4,  2, false } },
	{ 15, { 5,  0, false } },
	{ 16, { 5,  1, false } },
	{ 17, { 5,  2, false } },
	{ 18, { 6,  0, false } },
	{ 19, { 6,  1, false } },
	{ 20, { 6,  2, false } },
	{ 21, { 7,  0, true } },
	{ 22, { 7,  1, false } },
	{ 23, { 7,  2, false } },
	{ 24, { 8,  0, false } },
	{ 25, { 8,  1, false } },
	{ 26, { 8,  2, false } },
	{ 27, { 9,  0, false } },
	{ 28, { 9,  1, false } },
	{ 29, { 9,  2, false } },
	{ 30, { 10, 0, false } },
	{ 31, { 10, 1, false } },
	{ 32, { 10, 2, false } },
	{ 33, { 11, 0, true } },
	{ 34, { 11, 1, true } },
	{ 35, { 11, 2, true } },
	{ 36, { 12, 0, true } },
	{ 37, { 12, 1, true } },
	{ 38, { 12, 2, true } },
	{ 39, { 13, 0, false } },
	{ 40, { 13, 1, true } },
	{ 41, { 13, 2, false } },
};


std::map<std::string, int> mapFileNameToMapUniqueIDLookup
{
	{ "resources/western.csv", 0 },
};

std::map<int, std::map<int, TileInfo>> mapFileNameToTileIdToInfoLookup
{
	{ 0, westernTileIdToInfoLookup },
};

#pragma region Constructor

MapTile::MapTile(const std::string& mapFileNameName, const int id, const int worldGridRow, const int worldGridColumn)
	: id(id), worldGridRow(worldGridRow), worldGridColumn(worldGridColumn)
{
	this->mapUniqueId = mapFileNameToMapUniqueIDLookup[mapFileNameName];

	this->walkable = mapFileNameToTileIdToInfoLookup[this->mapUniqueId][this->id].walkAble;
}

#pragma endregion

#pragma region Public Methods

MapTile::~MapTile()
{
	
}

void MapTile::Draw(Texture* texture, int cameraShiftX, int cameraShiftY)
{
	Display::QueueTextureForRendering(texture, (this->worldGridColumn * TILE_WIDTH) - (TILE_WIDTH / 2) - cameraShiftX, (this->worldGridRow * TILE_HEIGHT) - (TILE_HEIGHT / 2) - cameraShiftY, TILE_WIDTH, TILE_HEIGHT, false, true, mapFileNameToTileIdToInfoLookup[this->mapUniqueId][this->id].spriteSheetColumnOffset * TILE_WIDTH, mapFileNameToTileIdToInfoLookup[this->mapUniqueId][this->id].spriteSheetRowOffset * TILE_HEIGHT);
}

int MapTile::GetWorldGridRow() const
{
	return this->worldGridRow;
}

int MapTile::GetWorldGridColumn() const
{
	return this->worldGridColumn;
}

bool MapTile::GetIsWalkable() const
{
	return this->walkable;
}

int MapTile::GetMapIdByFileName(const std::string& filename)
{
	return mapFileNameToMapUniqueIDLookup[filename];
}

#pragma endregion
