#include "Map.h"
#include "MapTile.h"
#include "Texture.h"
#include <fstream>
#include <sstream>

#ifdef _DEBUG
	#include <assert.h>
#endif

#define DEFAULT_EMPTY_MAP_TILE_ID 853

#pragma region Constructor

Map::Map(const std::string& tileDataFilePath, const std::string& textureFilepath)
{
	this->tileDataFilePath = tileDataFilePath;

	bool tileInitSuccess = MapTile::InitInteriorTileInfo();	//needs to happen before readDataFile below

	bool readDataSuccess = this->readDataFile(tileDataFilePath);

	this->texture = new Texture(textureFilepath);
	bool loadSuccess = this->texture->Load();

#if _DEBUG
	assert(readDataSuccess);
	assert(loadSuccess);
	assert(tileInitSuccess);
#endif
}

#pragma endregion

#pragma region Public Methods

Map::~Map()
{
	for (MapTile* tile : this->mapTiles)
	{
		delete tile;
	}
	this->mapTiles.clear();

	if (this->texture)
	{
		delete this->texture;
		this->texture = nullptr;
	}
}

void Map::Draw(int cameraShiftX, int cameraShiftY)
{
#if _DEBUG
	assert(this->rowCount > 0);
	assert(this->columnCount > 0);
	assert(this->mapTiles.size() > 0);
	assert(this->texture);
#endif

	for (MapTile* tile : this->mapTiles)
	{
		tile->Draw(this->texture, cameraShiftX, cameraShiftY);
	}
}

int Map::GetRowCount() const
{
	return this->rowCount;
}

int Map::GetColumnCount() const
{
	return this->columnCount;
}

const MapTile* Map::GetTileByWorldGridLocation(int row, int column) const
{
	for (const MapTile* tile : this->mapTiles)
	{
		if (tile->GetWorldGridRow() == row && tile->GetWorldGridColumn() == column)
			return tile;
	}

#if _DEBUG
	assert(false);	//we failed to find it!
#endif

	return nullptr;
}

#pragma endregion

#pragma region Private Methods

bool Map::readDataFile(const std::string& tileDatafilePath)
{
	std::ifstream file(tileDatafilePath.c_str());

	if (!file.is_open())
		return false;

	this->rowCount = 0;
	this->columnCount = 0;

	std::string line;
	while (std::getline(file, line))
	{
		if (line.length() == 0)
			continue;

		this->columnCount = 0;

		char* l = _strdup(line.c_str());

		char* context = NULL;
		char* token = strtok_s(l, ",", &context);
		while (token != NULL)
		{
			int id = atoi(token);
			
			if (id < 0)
				id = DEFAULT_EMPTY_MAP_TILE_ID;

			MapTile* tile = new MapTile(this->tileDataFilePath, id, this->rowCount, this->columnCount);
			this->mapTiles.push_back(tile);

			token = strtok_s(NULL, ",", &context);
			this->columnCount++;
		}

		free(l);
		this->rowCount++;
	}

	file.close();

#if _DEBUG
	assert(this->mapTiles.size() == (this->rowCount * this->columnCount));
#endif

	return true;
}

#pragma endregion
