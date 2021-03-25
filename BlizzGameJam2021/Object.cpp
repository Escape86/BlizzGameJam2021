#include "Object.h"
#include "Texture.h"
#include "Display.h"

#ifdef _DEBUG
	#include <assert.h>
#endif

#pragma region Constructor

Object::Object(double spawnX, double spawnY, int width, int height, const std::string& texturePath)
	: width(width), height(height)
{
#if _DEBUG
	assert(this->width);
	assert(this->height);
#endif

	this->x = spawnX;
	this->y = spawnY;

	this->SetTexture(texturePath);

	// fix the rendering space
	this->texture->SetRenderOffset(this->width / 2, this->height / 2);
}

#pragma endregion

#pragma region Public Methods

Object::~Object()
{
	if (this->texture)
	{
		delete this->texture;
	}
}

void Object::Draw()
{
#if _DEBUG
	assert(this->width);
	assert(this->height);
#endif

	Display::QueueTextureForRendering(this->texture, this->x, this->y, this->width, this->height, true);
}

bool Object::TestCollision(const Object* otherObject) const
{
#if _DEBUG
	assert(this->width);
	assert(this->height);
#endif

	const int thisHalfWidth = this->width / 2;
	const int thisHalfHeight = this->height / 2;
	const int otherHalfWidth = otherObject->width / 2;
	const int otherHalfHeight = otherObject->height / 2;

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
	leftB = otherObject->x - otherHalfWidth;
	rightB = otherObject->x + otherHalfWidth;
	topB = otherObject->y - otherHalfHeight;
	bottomB = otherObject->y + otherHalfHeight;

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

bool Object::TestCollisionWithRect(const SDL_Rect* thing) const
{
#if _DEBUG
	assert(this->width);
	assert(this->height);
#endif

	const int thisHalfWidth = this->width / 2;
	const int thisHalfHeight = this->height / 2;
	const int otherHalfWidth = thing->w / 2;
	const int otherHalfHeight = thing->h / 2;

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
	leftB = thing->x - otherHalfWidth;
	rightB = thing->x + otherHalfWidth;
	topB = thing->y - otherHalfHeight;
	bottomB = thing->y + otherHalfHeight;

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

void Object::SetTexture(const std::string& texturePath)
{
	if (this->texture)
	{
		delete this->texture;
	}

	this->texture = new Texture(texturePath);
	bool loadTextureResult = this->texture->Load();

#if _DEBUG
	assert(loadTextureResult);
#endif
}

double Object::GetPositionX() const
{
	return this->x;
}

double Object::GetPositionY() const
{
	return this->y;
}

void Object::SetPosition(double x, double y)
{
	this->x = x;
	this->y = y;
}

Direction Object::GetFacing() const
{
	return this->facing;
}

#pragma endregion
