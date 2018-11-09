#pragma once
#include "Scene.h"
#include <iostream>
#include <SDL.h>

class ColliderComponent;
class AssetManager;
class Manager;
class Vector2D;

class GameScene : public Scene {
public:
	GameScene(){}
	void update();
	void render();
	void init();
	static Vector2D mPlayerPos;
	static time_t lastLostLife;
	static time_t enemyTimer;
	static int enemiesSpawned;
	static int enemyLevel;
	static int enemiesLeftToKill;
	static int score;
	static int enemyTime;
	bool sandwichOn = false;

	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupColliders,
		groupProjectiles,
		groupScarecrows
	};

private:

};