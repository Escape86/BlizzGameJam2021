#pragma once

#include <string>
#include <vector>
#include <map>

#pragma region Forward Declarations
class Texture;
class MapTile;
#pragma endregion

class Map
{
public:
	Map(const std::vector<std::string>& tileDataFilePathsByLayer, const std::string& textureFilepath);
	~Map();

	void Draw(int cameraShiftX, int cameraShiftY) const;

	int GetRowCount() const;
	int GetColumnCount() const;
	int GetNumberOfLayers() const;
	const MapTile* GetTileByWorldGridLocation(int row, int column, int layer) const;

private:
	bool readDataFile(const std::string& tileDataFilepath, int layer);

	int rowCount = 0;
	int columnCount = 0;
	Texture* texture;
	std::map<int, std::vector<MapTile*>> mapTilesByLayer;
};