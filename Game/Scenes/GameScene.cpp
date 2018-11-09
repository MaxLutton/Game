#include "GameScene.h"
#include "../ECS.h"
#include "../Map.h"
#include "../Components.h"
#include "../Collision.h"
#include <sstream>


extern Manager manager;
Map* map;

auto& player(manager.addEntity());
auto& label(manager.addEntity());
auto& gameOver(manager.addEntity());
auto& nextLevel(manager.addEntity());
auto& scarecrow(manager.addEntity());
auto& sandwich(manager.addEntity());
auto& controls(manager.addEntity());

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& scarecrows(manager.getGroup(Game::groupScarecrows)); // <- getting all the scarecrow entities from manager

Vector2D GameScene::mPlayerPos;
time_t GameScene::lastLostLife;
time_t GameScene::enemyTimer;
int GameScene::enemiesSpawned;
int GameScene::enemyLevel;
int GameScene::enemiesLeftToKill;
int GameScene::score;
int GameScene::enemyTime;

void GameScene::init()
{
	lastLostLife = time(0);

	Game::scene = 1;
	map = new Map("terrain", 3, 32);

	//ECS implementation

	map->loadMap("assets/map.map", 25, 20);
	//map->loadMap("assets/ugly_collision_map.txt", 20, 8);

	player.addComponent<TransformComponent>(800, 640, 32, 32, 4);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);
	player.addComponent<HealthComponent>(3, "player");

	scarecrow.addComponent<TransformComponent>(1000, 640, 32, 32, 4);
	scarecrow.addComponent<SpriteComponent>("scarecrow", true);
	scarecrow.addComponent<ColliderComponent>("scarecrow"); // <- just adding in a tag
	scarecrow.addGroup(groupScarecrows);
	scarecrow.addComponent<RandomWalkComponent>();
	scarecrow.addComponent<HealthComponent>(1, "scarecrow");
	enemyTimer = time(0);

	SDL_Color white = { 255, 255, 255, 255 };
	label.addComponent<UILabel>(10, 10, "Test String", "arial", white);
	controls.addComponent<UILabel>(10, 620, "CONTROLS: move: w-a-s-d shoot: space quit: ESC", "arial", white);

	enemiesSpawned = 1;
	enemyLevel = 2;
	enemiesLeftToKill = enemyLevel;
	enemyTime = 4;

	sandwich.addComponent<TransformComponent>(-1, 320, 32, 32, 4);
	sandwich.addComponent<SpriteComponent>("sandwich", true, true);
	sandwich.addComponent<ColliderComponent>("sandwich");
}

void GameScene::update()
{
	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	mPlayerPos = player.getComponent<TransformComponent>().position;
	int lives = player.getComponent<HealthComponent>().lives;
	std::stringstream ss;
	ss << " Daisy's Lives: " << lives << " Daisy's Score: " << score << " Stupid Scarecrows left: " << enemiesLeftToKill;
	label.getComponent<UILabel>().setLabelText(ss.str(), "arial");

	manager.refresh();
	manager.update();

	//for (auto& c : colliders)
	//{
	//	SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
	//	// If collision, just put keep player at position
	//	if (Collision::AABB(cCol, playerCol))
	//	{
	//		std::cout << "hit" << std::endl;
	//		player.getComponent<TransformComponent>().position = mPlayerPos;
	//	}
	//}

	for (auto& s : scarecrows)
	{
		SDL_Rect cCol = s->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol))
		{
			player.getComponent<SpriteComponent>().play("Explode");
			double timeDiff = difftime(time(0), lastLostLife);
			if (timeDiff > 2)
			{
				std::cout << "LOST A LIFE" << std::endl;
				player.getComponent<HealthComponent>().takeLife();
				lastLostLife = time(0);
			}
		}
	}


	srand(time(NULL));
	for (auto& s : scarecrows)
	{
		if (s->getComponent<RandomWalkComponent>().getNewWalk)
		{
			int rando = rand() % 8;
			if (rando == 0)
			{
				s->getComponent<RandomWalkComponent>().setNewWalk(Vector2D(1, 0), 100, 2);
			}
			else if (rando == 2)
			{
				s->getComponent<RandomWalkComponent>().setNewWalk(Vector2D(-1, 0), 100, 2);
			}
			else if (rando == 4)
			{
				s->getComponent<RandomWalkComponent>().setNewWalk(Vector2D(0, 1), 100, 2);
			}
			else if (rando == 6)
			{
				s->getComponent<RandomWalkComponent>().setNewWalk(Vector2D(0, -1), 100, 2);
			}
		}

	}

	for (auto& p : projectiles)
	{
		for (auto& s : scarecrows)
		{
			if (Collision::AABB(s->getComponent<ColliderComponent>().collider,
				p->getComponent<ColliderComponent>().collider))
			{
				std::cout << "Hit A Stupid Scarecrow!" << std::endl;
				p->destroy();
				s->getComponent<HealthComponent>().takeLife();
				s->getComponent<SpriteComponent>().play("Explode");
				score++;
			}
		}

	}


	Game::camera.x = player.getComponent<TransformComponent>().position.x - 400;
	Game::camera.y = player.getComponent<TransformComponent>().position.y - 320;

	if (Game::camera.x < 0)
		Game::camera.x = 0;
	if (Game::camera.y < 0)
		Game::camera.y = 0;
	if (Game::camera.x > Game::camera.w)
		Game::camera.x = Game::camera.w;
	if (Game::camera.y > Game::camera.h)
		Game::camera.y = Game::camera.h;

	if (difftime(time(0), enemyTimer) > enemyTime && enemiesSpawned < enemyLevel)
	{
		auto& tempEnemy(manager.addEntity());
		tempEnemy.addComponent<TransformComponent>(1000, 640, 32, 32, 4);
		std::string name = "scarecrow" + std::to_string(enemiesSpawned);
		tempEnemy.addComponent<SpriteComponent>("scarecrow", true);
		tempEnemy.addComponent<ColliderComponent>(name); // <- just adding in a tag
		tempEnemy.addGroup(groupScarecrows);
		tempEnemy.addComponent<RandomWalkComponent>();
		tempEnemy.addComponent<HealthComponent>(1, name);
		enemyTimer = time(0);
		enemiesSpawned++;
	}

	if (player.getComponent<HealthComponent>().lives <= 0)
	{
		SDL_Color orange = { 255, 165, 0, 255 };
		gameOver.addComponent<UILabel>(400, 320, "GAME OVER", "arial", orange);
	}
	if (enemiesLeftToKill == 0 && !sandwichOn)
	{
		sandwichOn = true;
		sandwich.getComponent<TransformComponent>().position = Vector2D(player.getComponent<TransformComponent>().position.x - 200, player.getComponent<TransformComponent>().position.y);
		SDL_Color orange = { 255, 165, 0, 255 };
		nextLevel.addComponent<UILabel>(400, 320, "NEXT LEVEL STARTING NOW", "arial", orange);
	}

	if (sandwichOn)
	{

		if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
			sandwich.getComponent<ColliderComponent>().collider))
		{
			score += 10;
			enemyLevel *= 2;
			enemiesLeftToKill = enemyLevel;
			enemiesSpawned = 0;
			player.getComponent<HealthComponent>().lives = 3;
			if (enemyTime > 1)
				enemyTime--;
			sandwich.getComponent<TransformComponent>().position.x = -1;
			sandwichOn = false;
		}
	}
}

void GameScene::render()
{
	SDL_RenderClear(Game::renderer);
	// This is where we would add things to render

	for (auto& t : tiles)
	{
		t->draw();
	}
	for (auto& p : players)
	{
		p->draw();
	}
	for (auto& s : scarecrows)
	{
		s->draw();
	}
	//for (auto& c : colliders)
	//{
	//	c->draw();
	//}
	for (auto& p : projectiles)
	{
		p->draw();
	}

	if (player.getComponent<HealthComponent>().lives <= 0)
		gameOver.draw();
	label.draw();
	controls.draw();
	if (sandwichOn)
	{
		sandwich.draw();
		nextLevel.draw();
	}
	SDL_RenderPresent(Game::renderer);
}