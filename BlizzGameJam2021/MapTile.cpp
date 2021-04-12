#include "MapTile.h"
#include "Display.h"
#include "Constants.h"
#include <map>
#include <fstream>

#if _DEBUG
	#include <assert.h>
#endif

struct TileInfo
{
	int spriteSheetRowOffset;
	int spriteSheetColumnOffset;
	bool walkAble;
};

std::map<int, TileInfo> interiorTileIdToInfoLookup;		// { id, { spriteSheetRowOffset, spriteSheetColumnOffset, walkAble }}

bool MapTile::InitInteriorTileInfo()
{
	//read info from file
	std::fstream file;
	file.open(INTERIOR_TILESET_WALKABLE_DATA_FILEPATH, std::ios::in);
	
	if (!file.is_open())
		return false;

	std::map<int, bool> walkableValues;

	std::string buffer;
	while (std::getline(file, buffer))
	{
		size_t delimiterLocation = buffer.find(':');

		std::string idAsString = buffer.substr(0, delimiterLocation);
		int id = std::stoi(idAsString);

		std::string valueAsString = buffer.substr(delimiterLocation + 1);	//+1 to skip delimiter
		bool value = (valueAsString.compare("true") == 0);

		walkableValues.insert(std::make_pair(id, value));
	}

	file.close();

	const int ROW_COUNT = 61;
	const int COLUMN_COUNT = 14;

	for (int row = 0; row < ROW_COUNT; row++)
	{
		for (int column = 0; column < COLUMN_COUNT; column++)
		{
			int id = (row * COLUMN_COUNT) + column;
			interiorTileIdToInfoLookup.insert({ id, { row, column, walkableValues[id] } });
		}
	}

	return true;
}

const std::map<std::string, int> mapFileNameToMapUniqueIDLookup
{
	{ STARTING_HOUSE_MAP_DATA_FILEPATH0, 0 },
	{ STARTING_HOUSE_MAP_DATA_FILEPATH1, 0 },
	{ STARTING_HOUSE_MAP_DATA_FILEPATH2, 0 },

	{ TEST_BUILDING_MAP_DATA_FILEPATH0, 1 },
	{ TEST_BUILDING_MAP_DATA_FILEPATH1, 1 },
	{ TEST_BUILDING_MAP_DATA_FILEPATH2, 1 },
};

const std::map<int, const std::map<int, TileInfo>&> mapFileNameToTileIdToInfoLookup
{
	{ 0, interiorTileIdToInfoLookup },
	{ 1, interiorTileIdToInfoLookup },
};

#pragma region Constructor

MapTile::MapTile(const std::string& mapFileNameName, const int id, const int worldGridRow, const int worldGridColumn)
	: id(id), worldGridRow(worldGridRow), worldGridColumn(worldGridColumn)
{
	this->mapUniqueId = mapFileNameToMapUniqueIDLookup.at(mapFileNameName);

	this->walkable = mapFileNameToTileIdToInfoLookup.at(this->mapUniqueId).at(this->id).walkAble;
}

#pragma endregion

#pragma region Public Methods

MapTile::~MapTile()
{
	
}

void MapTile::Draw(Texture* texture, int cameraShiftX, int cameraShiftY) const
{
	Display::QueueTextureForRendering(texture, (this->worldGridColumn * TILE_WIDTH) - (TILE_WIDTH / 2) - cameraShiftX, (this->worldGridRow * TILE_HEIGHT) - (TILE_HEIGHT / 2) - cameraShiftY, TILE_WIDTH, TILE_HEIGHT, false, true, mapFileNameToTileIdToInfoLookup.at(this->mapUniqueId).at(this->id).spriteSheetColumnOffset * TILE_WIDTH, mapFileNameToTileIdToInfoLookup.at(this->mapUniqueId).at(this->id).spriteSheetRowOffset * TILE_HEIGHT);
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
	return mapFileNameToMapUniqueIDLookup.at(filename);
}

#pragma endregion
