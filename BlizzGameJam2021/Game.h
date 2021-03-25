#pragma once
#include "SDL_rect.h"

#pragma region Forward Declarations
class Player;
#pragma endregion

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

	const Player* GetPlayer() const;

	const SDL_Rect& GetCamera() const;

private:
	Player* player;
	
	SDL_Rect camera;

	unsigned int previousFrameEndTime;

	static Game* _instance;
};