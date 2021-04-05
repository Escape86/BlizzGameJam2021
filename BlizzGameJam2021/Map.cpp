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

Map::Map(const std::vector<std::string>& tileDataFilePathsByLayer, const std::string& textureFilepath)
{
	bool tileInitSuccess = MapTile::InitInteriorTileInfo();	//needs to happen before readDataFile below

	const int numberOfLayers = tileDataFilePathsByLayer.size();
	for (int layer = 0; layer < numberOfLayers; layer++)
	{
		const std::string& filepath = tileDataFilePathsByLayer.at(layer);
		bool readDataSuccess = this->readDataFile(filepath, layer);

#if _DEBUG
		assert(readDataSuccess);
#endif
	}

	this->texture = new Texture(textureFilepath);
	bool loadSuccess = this->texture->Load();

#if _DEBUG
	assert(loadSuccess);
	assert(tileInitSuccess);
#endif
}

#pragma endregion

#pragma region Public Methods

Map::~Map()
{
	for (std::pair<int, std::vector<MapTile*>> mapLayer : this->mapTilesByLayer)
	{
		std::vector<MapTile*>& mapTiles = mapLayer.second;

		for (MapTile* tile : mapTiles)
		{
			delete tile;
			tile = nullptr;
		}
		mapTiles.clear();
	}
	this->mapTilesByLayer.clear();

	if (this->texture)
	{
		delete this->texture;
		this->texture = nullptr;
	}
}

void Map::Draw(int cameraShiftX, int cameraShiftY) const
{
#if _DEBUG
	assert(this->rowCount > 0);
	assert(this->columnCount > 0);
	assert(this->mapTilesByLayer.size() > 0);
	assert(this->texture);
#endif

	for (std::pair<int, std::vector<MapTile*>> mapLayer : this->mapTilesByLayer)
	{
		const std::vector<MapTile*>& mapTiles = mapLayer.second;

		for (const MapTile* tile : mapTiles)
		{
			tile->Draw(this->texture, cameraShiftX, cameraShiftY);
		}
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

int Map::GetNumberOfLayers() const
{
	return this->mapTilesByLayer.size();
}

const MapTile* Map::GetTileByWorldGridLocation(int row, int column, int layer) const
{
#if _DEBUG
	assert(layer >= 0 && layer < this->mapTilesByLayer.size());
#endif

	for (const MapTile* tile : this->mapTilesByLayer.at(layer))
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

bool Map::readDataFile(const std::string& tileDataFilepath, int layer)
{
	std::ifstream file(tileDataFilepath.c_str());

	if (!file.is_open())
		return false;

	int fileRowCount = 0;
	int fileColumnCount = 0;
	std::vector<MapTile*> mapTiles;

	std::string line;
	while (std::getline(file, line))
	{
		if (line.length() == 0)
			continue;

		fileColumnCount = 0;

		char* l = _strdup(line.c_str());

		char* context = NULL;
		char* token = strtok_s(l, ",", &context);
		while (token != NULL)
		{
			int id = atoi(token);
			
			if (id < 0)
				id = DEFAULT_EMPTY_MAP_TILE_ID;

			MapTile* tile = new MapTile(tileDataFilepath, id, fileRowCount, fileColumnCount);
			mapTiles.push_back(tile);

			token = strtok_s(NULL, ",", &context);
			fileColumnCount++;
		}

		free(l);
		fileRowCount++;
	}

	file.close();

#if _DEBUG
	assert(mapTiles.size() == (fileRowCount * fileColumnCount));
#endif

	if (this->rowCount == 0)
	{
		this->rowCount = fileRowCount;
	}
	else
	{
#if _DEBUG
		assert(this->rowCount == fileRowCount);
#endif
	}

	if (this->columnCount == 0)
	{
		this->columnCount = fileColumnCount;
	}
	else
	{
#if _DEBUG
		assert(this->columnCount == fileColumnCount);
#endif
	}

	this->mapTilesByLayer.insert(std::make_pair(layer, mapTiles));

	return true;
}

#pragma endregion
