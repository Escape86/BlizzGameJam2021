#pragma once

#pragma region Filepaths
#define FONT_FILEPATH					"../resources/Fonts/Orbitron/Orbitron-Regular.ttf"
#define PLAYER_TEXTURE_PATH				"../resources/player.png"
#define INTERIOR_MAP_DATA_FILEPATH0		"../resources/maps/starting_house_Base.csv"
#define INTERIOR_MAP_DATA_FILEPATH1		"../resources/maps/starting_house_Objects-low.csv"
#define INTERIOR_MAP_DATA_FILEPATH2		"../resources/maps/starting_house_Objects-high.csv"
#define INTERIOR_MAP_TEXTURE_FILEPATH	"../resources/raou_interior_tptileset/top-down interior_v2.png"
#define INTERIOR_MAP_WALKABLE_DATA_FILEPATH "../resources/interior_tileset_walkable.txt"
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

#define TILE_WIDTH						16
#define TILE_HEIGHT						16

#define PLAYER_WIDTH					24
#define PLAYER_HEIGHT					24

#define PLAYER_SPAWN_POSITION_X			250
#define PLAYER_SPAWN_POSITION_Y			250

#define PLAYER_VELOCITY					100
#define PLAYER_ANIMATION_COOLDOWN		85

#define JOYSTICK_DEAD_ZONE				8000

#define WINDOW_TITLE					"BlizzGameJam2021"