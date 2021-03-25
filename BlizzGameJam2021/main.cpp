#include "Display.h"
#include "Audio.h"
#include "Game.h"

int main(int argc, char* args[])
{
	if (!Display::Initialize())
	{
		return -1;
	}

	if (!Audio::Initialize())
	{
		return -1;
	}

	Game* game = new Game();

	bool keepRunning = true;

	Display::SetEventCallback([&keepRunning, &game](SDL_Event e)
	{
		switch (e.type)
		{
			case SDL_KEYDOWN:
			{
				if (e.key.repeat == 0)
				{
					if (e.key.keysym.sym == SDLK_ESCAPE)
					{
						keepRunning = false;
					}

					game->InjectKeyDown((int)e.key.keysym.sym);
				}
				break;
			}
			case SDL_KEYUP:
			{
				if (e.key.repeat == 0)
				{
					game->InjectKeyUp((int)e.key.keysym.sym);
				}
				break;
			}
			case SDL_JOYAXISMOTION:
			{
				//did the event occur on ControllerID 0 ?
				if (e.jaxis.which == 0)
				{
					//yup, so publish the event
					game->InjectControllerStickMovement(e.jaxis.axis, e.jaxis.value);
				}
				break;
			}
			case SDL_JOYBUTTONDOWN:
			{
				game->InjectKeyDown(32);
				break;
			}
			case SDL_JOYBUTTONUP:
			{
				game->InjectKeyUp(32);
				break;
			}
			case SDL_QUIT:
			{
				keepRunning = false;
				break;
			}
		}
	});

	//game loop
	while (keepRunning)
	{
		//update game objects
		game->InjectFrame();

		//draw the frame
		Display::InjectFrame();
	}

	delete game;

	if (!Audio::ShutDown())
	{
		return -1;
	}

	if (!Display::ShutDown())
	{
		return -1;
	}

	return 0;
}