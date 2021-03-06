#pragma once

#include "Object.h"

#pragma region Forward Declarations

#pragma endregion

class Player : public Object
{
public:
	Player(double spawnX, double spawnY, Direction initialFacing);
	~Player();

	void InjectFrame(unsigned int elapsedGameTime, unsigned int previousFrameTime) override;
	void Draw() override;
	void OnKeyDown(int key);
	void OnKeyUp(int key);

	int GetHp() const;
	void SetHp(int hp);

	void ResetHorizontalVelocity();
	void ResetVerticalVelocity();

private:
	void updateSpriteSheetOffsets();

	int horizontalVelocity;
	int verticalVelocity;

	int hp;

	bool keydownPrimed;
	bool animationFlag;
	int animationSwapCooldown;
};