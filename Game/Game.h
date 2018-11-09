#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>

class ColliderComponent;
class AssetManager;
class Manager;
class Vector2D;
class Scene;

class Game {
public:
	Game();
	~Game();

	void init(const char* title, int width, int height, bool fullscreen);
	//void beginGame();
	void handleEvents();
	void update();
	void render();
	void clean();
	bool running() { return isRunning; }

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static SDL_Rect camera;
	static bool isRunning;
	static AssetManager* assets;
	static Vector2D mPlayerPos;
	static time_t lastLostLife;
	static time_t enemyTimer;
	static int enemiesSpawned;
	static int enemyLevel;
	static int enemiesLeftToKill;
	static int score;
	static int enemyTime;
	bool sandwichOn = false;
	static int scene;
	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupColliders,
		groupProjectiles,
		groupScarecrows
	};

		static Scene* currentScene;

private:
	int count;
	SDL_Window* window;
};


