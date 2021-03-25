#pragma once
#include "SDL_rect.h"

class Game
{
public:
	Game();
	~Game();

	static const Game* GetInstance();

	void InjectFrame();
	void InjectKeyDown(int key);
	void InjectKeyUp(int key);
	void InjectControllerStickMovement(unsigned char axis, short value);

	const SDL_Rect& GetCamera() const;

private:
	SDL_Rect camera;

	unsigned int previousFrameEndTime;

	static Game* _instance;
};