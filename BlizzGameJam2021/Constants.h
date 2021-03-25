#pragma once

#pragma region Filepaths
#define FONT_FILEPATH						"../resources/Fonts/Orbitron/Orbitron-Regular.ttf"
#pragma endregion

#pragma region Colors
#define BLUE							{ 0, 0, 255 }
#define GREEN							{ 0, 255, 0 }
#define RED								{ 255, 0, 0 }
#define BLACK							{ 0, 0, 0 }
#define WHITE							{ 255, 255, 255}
#pragma endregion

#define RENDER_SCALE_AMOUNT				2.0f	//if you change this you'll need to change the values in Game.cpp that enforce camera bounds

#define SCREEN_WIDTH					800
#define SCREEN_HEIGHT					600

#define JOYSTICK_DEAD_ZONE				8000

#define WINDOW_TITLE					"BlizzGameJam2021"