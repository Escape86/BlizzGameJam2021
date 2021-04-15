#pragma once

#pragma region Filepaths
#define FONT_FILEPATH								"../resources/Fonts/Orbitron/Orbitron-Regular.ttf"
#define HEART_TEXTURE_PATH							"../resources/Hearts.png"
#define PLAYER_TEXTURE_PATH							"../resources/player.png"
#define STARTING_HOUSE_MAP_DATA_FILEPATH0			"../resources/maps/starting_house_Base.csv"
#define STARTING_HOUSE_MAP_DATA_FILEPATH1			"../resources/maps/starting_house_Objects-low.csv"
#define STARTING_HOUSE_MAP_DATA_FILEPATH2			"../resources/maps/starting_house_Objects-high.csv"
#define STARTING_HOUSE_MAP_TELEPORTERS_FILEPATH		"../resources/maps/starting_house_teleporters.txt"
#define STARTING_HOUSE_MAP_SPAWNS_FILEPATH			"../resources/maps/starting_house_spawns.txt"
#define INTERIOR_TILESET_WALKABLE_DATA_FILEPATH		"../resources/interior_tileset_walkable.txt"
#define INTERIOR_TILESET_ISOBJECT_DATA_FILEPATH		"../resources/interior_tileset_isObject.txt"
#define INTERIOR_TILESET_TEXTURE_FILEPATH			"../resources/raou_interior_tptileset/top-down_interior_v2.png"
#define TEST_BUILDING_MAP_DATA_FILEPATH0			"../resources/maps/test_building_Base.csv"
#define TEST_BUILDING_MAP_DATA_FILEPATH1			"../resources/maps/test_building_Objects-low.csv"
#define TEST_BUILDING_MAP_DATA_FILEPATH2			"../resources/maps/test_building_Objects-high.csv"
#define TEST_BUILDING_MAP_TELEPORTERS_FILEPATH		"../resources/maps/test_building_teleporters.txt"
#define TEST_BUILDING_MAP_SPAWNS_FILEPATH			"../resources/maps/test_building_spawns.txt"
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

#define TELEPORTER_WIDTH				TILE_WIDTH
#define TELEPORTER_HEIGHT				1		//1 so you have to slightly walk into the door/portal area to actually be teleported

#define PLAYER_WIDTH					24
#define PLAYER_HEIGHT					24

#define PLAYER_SPAWN_POSITION_X			250
#define PLAYER_SPAWN_POSITION_Y			250

#define PLAYER_VELOCITY					100
#define PLAYER_ANIMATION_COOLDOWN		85
#define NPC_VELOCITY					50
#define NPC_IDLEMOVEMENT_COOLDOWN		1200	//in milliseconds

#define ENEMY_VELOCITY					50

#define PLAYER_MAX_HP					6
#define ENEMY_HP						3

#define ATTACK_RECOIL_AMOUNT			20

#define PLAYER_TAKE_DAMAGE_COOLDOWN		1000	//in milliseconds
#define ON_DAMAGE_VISIBILITY_REDUCE		75
#define VISIBILITY_RESTORE_COOLDOWN		1000	//in milliseconds

#define JOYSTICK_DEAD_ZONE				8000

#define WINDOW_TITLE					"BlizzGameJam2021"