#pragma once
#include "SDL_rect.h"
#include "Teleporter.h"
#include "Spawn.h"
#include <vector>
#include <string>

#pragma region Forward Declarations
class Player;
class Map;
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

	bool SwitchMap(const std::vector<std::string>& mapFilePathsByLayer, const std::string& mapTextureFilePath, const std::string& teleportersFilePath, const std::string& spawnsFilePath);

	const Player* GetPlayer() const;

	const Map* GetMap() const;

	const SDL_Rect& GetCamera() const;

private:

	void cleanUpGameObjects();
	bool loadTeleporters(const std::string& filepath);
	bool loadSpawns(const std::string& filepath);

	Player* player;
	Map* map;
	
	SDL_Rect camera;

	std::vector<Spawn> spawns;
	std::vector<Teleporter> teleporters;

	Destination destinationMapSwitch;
	bool mapSwitchRequested = false;

	unsigned int previousFrameEndTime;

	static Game* _instance;
};