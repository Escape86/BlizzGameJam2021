#pragma once

#include <vector>
#include <string>

#pragma region Forward Declarations
class Object;
#pragma endregion

struct Destination
{
	std::vector<std::string> destinationMapFilePathsByLayer;
	std::string destinationMapTextureFilePath;
	std::string destinationTeleportersFilePath;
	std::string destinationSpawnsFilePath;
	int destinationX;
	int destinationY;
};

class Teleporter
{
public:
	Teleporter(int x, int y, int width, int height, const std::vector<std::string>& destinationMapFilePathsByLayer, const std::string& destinationMapTextureFilePath, const std::string& destinationTeleportersFilePath, const std::string& destinationSpawnsFilePath, int destinationX, int destinationY);
	~Teleporter();

	bool TestCollision(const Object* otherObject) const;
	const Destination& GetDestination() const;

private:
	int x;
	int y;
	int width;
	int height;

	Destination destination;
};