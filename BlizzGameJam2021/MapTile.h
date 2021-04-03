#pragma once

#include <string>

#pragma region Forward Declarations
class Texture;
#pragma endregion

class MapTile
{
public:
	MapTile(const std::string& mapFileNameName, const int id, const int worldGridRow, const int worldGridColumn);
	~MapTile();

	void Draw(Texture* texture, int cameraShiftX, int cameraShiftY);

	int GetWorldGridRow() const;
	int GetWorldGridColumn() const;

	bool GetIsWalkable() const;

	static int GetMapIdByFileName(const std::string& filename);

	static bool InitInteriorTileInfo();

private:
	int mapUniqueId;
	const int id;
	const int worldGridRow;
	const int worldGridColumn;
	bool walkable;
};