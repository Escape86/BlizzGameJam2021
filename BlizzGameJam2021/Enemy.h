#pragma once

#include "Spawn.h"
#include <string>

class Enemy : public Spawn
{
public:
	Enemy(int id, double spawnX, double spawnY, int width, int height, const std::string& texturePath, int spriteSheetOffsetX, int spriteSheetOffsetY, bool shouldIdleMove);
	~Enemy();

	void InjectFrame(unsigned int elapsedGameTime, unsigned int previousFrameTime) override;

	void OnHitByPlayerAttack();

	int GetHP();
	void DoRecoil(Direction attackerIsFacing);

private:
	int hp;
};