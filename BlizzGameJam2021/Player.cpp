#include "Player.h"
#include "Game.h"
#include "Display.h"
#include "Constants.h"

#if _DEBUG
	#include <assert.h>
	
	int debug_player_pos_text_id = -1;
#endif

#pragma region Constructor

Player::Player(double spawnX, double spawnY, Direction initialFacing) : Object(spawnX, spawnY, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_TEXTURE_PATH)
{
	this->horizontalVelocity = 0;
	this->verticalVelocity = 0;

	this->spriteSheetOffsetX = 0;
	this->spriteSheetOffsetY = 0;

	this->facing = initialFacing;

#if _DEBUG
	debug_player_pos_text_id = Display::CreateText("(0,0)", 0, 288, Display::TWELVE, false);
#endif
}

#pragma endregion

#pragma region Public Methods

Player::~Player()
{
#if _DEBUG
	Display::RemoveText(debug_player_pos_text_id);
#endif
}

void Player::InjectFrame(unsigned int elapsedGameTime, unsigned int previousFrameTime)
{
	double previousFrameTimeInSeconds = (previousFrameTime / 1000.0);

	double startPosX = this->x;
	double startPosY = this->y;

	//update position
	this->x += (this->horizontalVelocity * previousFrameTimeInSeconds);
	this->y += (this->verticalVelocity * previousFrameTimeInSeconds);
}

void Player::Draw()
{ 
	this->updateSpriteSheetOffsets();

	const Game* game = Game::GetInstance();
	const SDL_Rect& camera = game->GetCamera();

	Display::QueueTextureForRendering(this->texture, this->x - camera.x, this->y - camera.y, this->width, this->height, true, true, this->spriteSheetOffsetX, this->spriteSheetOffsetY);

	//debug position text
#if _DEBUG
	std::string playerPosText = "(";
	playerPosText.append(std::to_string((int)this->GetPositionX()));
	playerPosText.append(", ");
	playerPosText.append(std::to_string((int)this->GetPositionY()));
	playerPosText.append(")");
	Display::UpdateText(debug_player_pos_text_id, playerPosText);
#endif
}

void Player::OnKeyDown(int key)
{
	switch (key)
	{
		case SDLK_w:
		case SDLK_UP:
			this->verticalVelocity -= PLAYER_VELOCITY;
			this->facing = Direction::UP;
			break;
		case SDLK_s:
		case SDLK_DOWN:
			this->verticalVelocity += PLAYER_VELOCITY;
			this->facing = Direction::DOWN;
			break;
		case SDLK_a:
		case SDLK_LEFT:
			this->horizontalVelocity -= PLAYER_VELOCITY;
			this->facing = Direction::LEFT;
			break;
		case SDLK_d:
		case SDLK_RIGHT:
			this->horizontalVelocity += PLAYER_VELOCITY;
			this->facing = Direction::RIGHT;
			break;
	}

	//enforce velocity min/max values
	if (this->verticalVelocity > PLAYER_VELOCITY)
		this->verticalVelocity = PLAYER_VELOCITY;
	else if (this->verticalVelocity < -PLAYER_VELOCITY)
		this->verticalVelocity = -PLAYER_VELOCITY;

	if (this->horizontalVelocity > PLAYER_VELOCITY)
		this->horizontalVelocity = PLAYER_VELOCITY;
	else if (this->horizontalVelocity < -PLAYER_VELOCITY)
		this->horizontalVelocity = -PLAYER_VELOCITY;
}

void Player::OnKeyUp(int key)
{
	switch (key)
	{
		case SDLK_w:
		case SDLK_UP:
			this->verticalVelocity += PLAYER_VELOCITY;
			break;
		case SDLK_s:
		case SDLK_DOWN:
			this->verticalVelocity -= PLAYER_VELOCITY;
			break;
		case SDLK_a:
		case SDLK_LEFT:
			this->horizontalVelocity += PLAYER_VELOCITY;
			break;
		case SDLK_d:
		case SDLK_RIGHT:
			this->horizontalVelocity -= PLAYER_VELOCITY;
			break;
	}

	//enforce velocity min/max values
	if (this->verticalVelocity > PLAYER_VELOCITY)
		this->verticalVelocity = PLAYER_VELOCITY;
	else if (this->verticalVelocity < -PLAYER_VELOCITY)
		this->verticalVelocity = -PLAYER_VELOCITY;

	if (this->horizontalVelocity > PLAYER_VELOCITY)
		this->horizontalVelocity = PLAYER_VELOCITY;
	else if (this->horizontalVelocity < -PLAYER_VELOCITY)
		this->horizontalVelocity = -PLAYER_VELOCITY;
}

void Player::ResetHorizontalVelocity()
{
	this->horizontalVelocity = 0;
}

void Player::ResetVerticalVelocity()
{
	this->verticalVelocity = 0;
}

#pragma endregion

#pragma region Private Methods

void Player::updateSpriteSheetOffsets()
{
	if (this->verticalVelocity || this->horizontalVelocity)
	{
		//is moving so show the appropriate section of the sprite sheet
		switch (this->facing)
		{
		case Direction::UP:
			{
				this->spriteSheetOffsetX = PLAYER_WIDTH;
				this->spriteSheetOffsetY = PLAYER_HEIGHT;
			}
			break;
		case Direction::DOWN:
			{
				this->spriteSheetOffsetX = PLAYER_WIDTH;
				this->spriteSheetOffsetY = 0;
			}
			break;
		case Direction::LEFT:
			{
				this->spriteSheetOffsetX = PLAYER_WIDTH * 2;
				this->spriteSheetOffsetY = PLAYER_HEIGHT;
			}
			break;
		case Direction::RIGHT:
			{
				this->spriteSheetOffsetX = 0;
				this->spriteSheetOffsetY = PLAYER_HEIGHT * 2;
			}
			break;

		default:
#if _DEBUG
	assert(false);	//wtf direction is this?
#endif
			break;
		}
	}
	else
	{
		//is not moving, so show the idle section of the sprite sheet
		switch (this->facing)
		{
		case Direction::UP:
				this->spriteSheetOffsetX = 0;
				this->spriteSheetOffsetY = PLAYER_HEIGHT * 3;
			break;
		case Direction::DOWN:
				this->spriteSheetOffsetX = PLAYER_WIDTH * 2;
				this->spriteSheetOffsetY = PLAYER_HEIGHT * 2;
			break;
		case Direction::LEFT:
				this->spriteSheetOffsetX = PLAYER_WIDTH * 2;
				this->spriteSheetOffsetY = PLAYER_HEIGHT * 3;
			break;
		case Direction::RIGHT:
				this->spriteSheetOffsetX = PLAYER_WIDTH;
				this->spriteSheetOffsetY = PLAYER_HEIGHT * 3;
			break;

		default:
#if _DEBUG
	assert(false);	//wtf direction is this?
#endif
			break;
		}
	}
}

#pragma endregion