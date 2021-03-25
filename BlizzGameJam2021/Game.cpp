#include "Game.h"
#include "Constants.h"
#include "SDL_timer.h"
#include "SDL_keycode.h"

#if _DEBUG
	#include <assert.h>
#endif

Game* Game::_instance;

#pragma region Constructor

Game::Game()
	: previousFrameEndTime(0)
{
#if _DEBUG
	assert(Game::_instance == nullptr);	//already initialized!
#endif	

	//init camera
	this->camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	Game::_instance = this;
}

#pragma endregion

#pragma region Public Methods

Game::~Game()
{

}

const Game* Game::GetInstance()
{
	return Game::_instance;
}

void Game::InjectFrame()
{
	Uint32 elapsedTimeInMilliseconds = SDL_GetTicks();

	const unsigned int previousFrameTime = elapsedTimeInMilliseconds - this->previousFrameEndTime;

	float frameTimeInMilliseconds = previousFrameTime / 1000.0f;

	
	//end of frame
	this->previousFrameEndTime = elapsedTimeInMilliseconds;
}

void Game::InjectKeyDown(int key)
{
	
}

void Game::InjectKeyUp(int key)
{

}

void Game::InjectControllerStickMovement(unsigned char axis, short value)
{
	//X axis motion
	if (axis == 0)
	{
		//Left of dead zone
		if (value < -JOYSTICK_DEAD_ZONE)
		{

		}
		//Right of dead zone
		else if (value > JOYSTICK_DEAD_ZONE)
		{

		}
		//In horizontal dead zone
		else
		{

		}
	}
	//Y axis motion
	else if (axis == 1)
	{
		//Above dead zone
		if (value < -JOYSTICK_DEAD_ZONE)
		{

		}
		//Below dead zone
		else if (value > JOYSTICK_DEAD_ZONE)
		{
			
		}
		//In vertical dead zone
		else
		{

		}
	}
}

const SDL_Rect& Game::GetCamera() const
{
	return this->camera;
}

#pragma endregion

#pragma region Private Methods

#pragma endregion