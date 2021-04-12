#include "Teleporter.h"
#include "Object.h"

#if _DEBUG
	#include <assert.h>
#endif

#pragma region Constructor

Teleporter::Teleporter(int x, int y, int width, int height, const std::vector<std::string>& destinationMapFilePathsByLayer, const std::string& destinationMapTextureFilePath, const std::string& destinationTeleportersFilePath, int destinationX, int destinationY)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->destination = 
	{
		destinationMapFilePathsByLayer,
		destinationMapTextureFilePath,
		destinationTeleportersFilePath,
		destinationX,
		destinationY
	};
}

#pragma endregion

#pragma region Public Methods

Teleporter::~Teleporter()
{

}

bool Teleporter::TestCollision(const Object* otherObject) const
{
#if _DEBUG
	assert(this->width);
	assert(this->height);
#endif

	const int thisHalfWidth = this->width / 2;
	const int thisHalfHeight = this->height / 2;
	const int otherHalfWidth = otherObject->GetWidth() / 2;
	const int otherHalfHeight = otherObject->GetHeight() / 2;

	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = this->x - thisHalfWidth;
	rightA = this->x + thisHalfWidth;
	topA = this->y - thisHalfHeight;
	bottomA = this->y + thisHalfHeight;

	//Calculate the sides of rect B
	leftB = otherObject->GetPositionX() - otherHalfWidth;
	rightB = otherObject->GetPositionX() + otherHalfWidth;
	topB = otherObject->GetPositionY() - otherHalfHeight;
	bottomB = otherObject->GetPositionY() + otherHalfHeight;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

const Destination& Teleporter::GetDestination() const
{
	return this->destination;
}

#pragma endregion