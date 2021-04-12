#include "Enemy.h"
#include "Game.h"
#include "Player.h"
#include "Constants.h"

#if _DEBUG
	#include <assert.h>
#endif

#pragma region Constructor

Enemy::Enemy(int id, double spawnX, double spawnY, int width, int height, const std::string& texturePath, int spriteSheetOffsetX, int spriteSheetOffsetY, bool shouldIdleMove)
	: Spawn(id, spawnX, spawnY, width, height, texturePath, spriteSheetOffsetX, spriteSheetOffsetY, shouldIdleMove)
{
	this->hp = ENEMY_HP;
}

#pragma endregion

#pragma region Public Methods

Enemy::~Enemy()
{

}

void Enemy::InjectFrame(unsigned int elapsedGameTime, unsigned int previousFrameTime)
{
	float previousFrameTimeInSeconds = (previousFrameTime / 1000.0f);

	//either chase player or move randomly around if told to use idle movement
	if (this->shouldIdleMove)
	{
		Spawn::InjectFrame(elapsedGameTime, previousFrameTime);
		return;
	}
	
	//else do chase player
	const Player* player = Game::GetInstance()->GetPlayer();
	
#if _DEBUG
	assert(player);
#endif

	int targetX = player->GetPositionX();
	int targetY = player->GetPositionY();

	float movementVelocity = ENEMY_VELOCITY * previousFrameTimeInSeconds;

	if (this->x > targetX)
	{
		this->x -= movementVelocity;
	}
	else if (this->x < targetX)
	{
		this->x += movementVelocity;
	}

	if (this->y > targetY)
	{
		this->y -= movementVelocity;
	}
	else if (this->y < targetY)
	{
		this->y += movementVelocity;
	}
}
void Enemy::OnHitByPlayerAttack()
{
	this->hp--;
}

int Enemy::GetHP()
{
	return this->hp;
}

void Enemy::DoRecoil(Direction attackerIsFacing)
{
	//bounce back to show recoil
	switch (attackerIsFacing)
	{
	case Direction::UP:
		this->y -= ATTACK_RECOIL_AMOUNT;
		break;
	case Direction::DOWN:
		this->y += ATTACK_RECOIL_AMOUNT;
		break;
	case Direction::LEFT:
		this->x -= ATTACK_RECOIL_AMOUNT;
		break;
	case Direction::RIGHT:
		this->x += ATTACK_RECOIL_AMOUNT;
		break;

	default:
#if _DEBUG
		assert(false);	//wtf direction?
#endif
		break;
	}
}

#pragma endregion