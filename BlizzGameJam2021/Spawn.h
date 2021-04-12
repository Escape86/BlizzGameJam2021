#pragma once

#include "Object.h"

class Spawn : public Object
{
public:
	Spawn(int id, double spawnX, double spawnY, int width, int height, const std::string& texturePath, int spriteSheetOffsetX, int spriteSheetOffsetY);
	~Spawn();

	void InjectFrame(unsigned int elapsedGameTime, unsigned int previousFrameTime) override;
	void Draw() override;

	int GetID();

private:
	const int id;
};