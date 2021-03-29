#pragma once

#include <string>
#include <vector>

#pragma region Forward Declarations
class Texture;
class MapTile;
#pragma endregion

class Map
{
public:
	Map(const std::string& tileDataFilePath, const std::string& textureFilepath);
	~Map();

	void Draw(int cameraShiftX, int cameraShiftY);

	int GetRowCount() const;
	int GetColumnCount() const;
	const MapTile* GetTileByWorldGridLocation(int row, int column) const;

private:
	void readDataFile(const std::string& tileDatafilePath);

	std::string tileDataFilePath;
	int rowCount;
	int columnCount;
	Texture* texture;
	std::vector<MapTile*> mapTiles;
};