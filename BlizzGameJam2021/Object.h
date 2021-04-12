#pragma once

#include <string>

#pragma region Forward Declarations
class Texture;
struct SDL_Rect;
#pragma endregion

enum Direction
{
	NONE = -1,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Object
{
public:
	Object(double spawnX, double spawnY, int width, int height, const std::string& texturePath);
	virtual ~Object();

	virtual void InjectFrame(unsigned int elapsedGameTime, unsigned int previousFrameTime) = 0;

	virtual void Draw();

	bool TestCollision(const Object* otherObject) const;
	bool TestCollisionWithRect(const SDL_Rect* thing) const;
	void SetTexture(const std::string& texturePath);

	double GetPositionX() const;
	double GetPositionY() const;

	int GetWidth() const;
	int GetHeight() const;

	void SetPosition(double x, double y);

	Direction GetFacing() const;

protected:
	double x;
	double y;
	const int width;
	const int height;
	Direction facing;
	Texture* texture = nullptr;
	int spriteSheetOffsetX;
	int spriteSheetOffsetY;
};