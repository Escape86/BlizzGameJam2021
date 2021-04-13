#include "Player.h"
#include "Game.h"
#include "Map.h"
#include "MapTile.h"
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

	this->hp = PLAYER_MAX_HP;

	this->animationFlag = false;
	this->animationSwapCooldown = 0;

	this->facing = initialFacing;

	//prevent level switching from causing keyups to occur without a corresponding keydown
	this->keydownPrimed = false;

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
	int startTileRow = static_cast<int>((this->y + (this->height / 2)) / TILE_HEIGHT);
	int startTileColumn = static_cast<int>((this->x + (this->width / 2)) / TILE_WIDTH);

	//update position
	this->x += (this->horizontalVelocity * previousFrameTimeInSeconds);
	this->y += (this->verticalVelocity * previousFrameTimeInSeconds);

	//enforce screen bounds
	int halfWidth = this->width / 2;
	int halfHeight = this->height / 2;

	const Game* game = Game::GetInstance();
	const Map* map = game->GetMap();
	const int mapWidth = map->GetColumnCount() * TILE_WIDTH;
	const int mapHeight = map->GetRowCount() * TILE_HEIGHT;

	if (this->x - halfWidth < 0)
	{
		this->x = halfWidth;
	}
	else if (this->x + halfWidth > mapWidth)
	{
		this->x = mapWidth - halfWidth;
	}

	if (this->y - halfHeight < 0)
	{
		this->y = halfHeight;
	}
	else if (this->y + halfHeight > mapHeight)
	{
		this->y = mapHeight - halfHeight;
	}

	//check if we're attempting to cross to a new tile that isn't walkable
	int endTileRow = static_cast<int>((this->y + (this->height / 2)) / TILE_HEIGHT);
	int endTileColumn = static_cast<int>((this->x + (this->width / 2)) / TILE_WIDTH);

	if (startTileRow != endTileRow || startTileColumn != endTileColumn)
	{
		//we crossed into a new tile, check if it's walkable
		const int numberOfMapLayers = map->GetNumberOfLayers();
		for (int layer = 0; layer < numberOfMapLayers; layer++)
		{
			//each layer of the map has different walkable data so have to check each layer
			const MapTile* tile = map->GetTileByWorldGridLocation(endTileRow, endTileColumn, layer);
			if (tile == nullptr || !tile->GetIsWalkable())
			{
				//not walkable, so move them back!
				this->x = startPosX;
				this->y = startPosY;
				break;
			}
		}
	}

	//animation
	if (this->animationSwapCooldown <= 0)
	{
		animationFlag = !animationFlag;
		this->animationSwapCooldown = PLAYER_ANIMATION_COOLDOWN;
	}
	else
	{
		this->animationSwapCooldown -= previousFrameTime;
	}
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

	this->keydownPrimed = true;
}

void Player::OnKeyUp(int key)
{
	if (!this->keydownPrimed)
		return;

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

int Player::GetHp() const
{
	return this->hp;
}

void Player::SetHp(int hp)
{
	this->hp = hp;
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
			if (animationFlag)
			{
				this->spriteSheetOffsetX = 0;
				this->spriteSheetOffsetY = PLAYER_HEIGHT;
			}
			else
			{
				this->spriteSheetOffsetX = PLAYER_WIDTH;
				this->spriteSheetOffsetY = PLAYER_HEIGHT;
			}
			break;
		case Direction::DOWN:
			if (animationFlag)
			{
				this->spriteSheetOffsetX = 0;
				this->spriteSheetOffsetY = 0;
			}
			else
			{
				this->spriteSheetOffsetX = PLAYER_WIDTH;
				this->spriteSheetOffsetY = 0;
			}
			break;
		case Direction::LEFT:
			if (animationFlag)
			{
				this->spriteSheetOffsetX = PLAYER_WIDTH * 2;
				this->spriteSheetOffsetY = 0;
			}
			else
			{
				this->spriteSheetOffsetX = PLAYER_WIDTH * 2;
				this->spriteSheetOffsetY = PLAYER_HEIGHT;
			}
			break;
		case Direction::RIGHT:
			if (animationFlag)
			{
				this->spriteSheetOffsetX = PLAYER_WIDTH;
				this->spriteSheetOffsetY = PLAYER_HEIGHT * 2;
			}
			else
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