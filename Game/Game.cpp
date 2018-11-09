#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <sstream>

// for random num
#include <stdlib.h> 
#include <time.h> 

Map* map;


Manager manager;
auto& player(manager.addEntity());
auto& label(manager.addEntity());
auto& welcome(manager.addEntity());
auto& gameOver(manager.addEntity());
auto& nextLevel(manager.addEntity());
auto& scarecrow(manager.addEntity());
auto& sandwich(manager.addEntity());
auto& controls(manager.addEntity());
Vector2D Game::mPlayerPos;

time_t Game::lastLostLife;
time_t Game::enemyTimer;
int Game::enemiesSpawned;
int Game::enemyLevel;
int Game::enemiesLeftToKill;
bool Game::goToMenu = true;
int Game::score = 0;
int Game::enemyTime;


SDL_Event Game::event;

SDL_Rect Game::camera = { 0, 0, 800, 640 };

AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;

Game::Game() 
{
	goToMenu = true;
}

Game::~Game() {}

void Game::init(const char* title, int width, int height, bool fullscreen) {

	lastLostLife = time(0);
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsystems Initalialized..." << std::endl;

		this->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		if (window) {
			std::cout << "Window Created..." << std::endl;
		}

		this->renderer = SDL_CreateRenderer(this->window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
			std::cout << "Renderer Created..." << std::endl;
		}

		this->isRunning = true;
	}
	else {
		this->isRunning = false;
	}

	if (TTF_Init() == -1)
	{
		std::cout << "Error: SDL_TTF" << std::endl;
	}

	assets->addFont("arial", "assets/arial.ttf", 16);
	assets->addFont("arial", "assets/arial.ttf", 100);


	//assets->addTexture("terrain", "assets/terrain_ss.png");
	////assets->addTexture("terrain", "assets/ugly_terrain.png");
	//assets->addTexture("player", "assets/player_animations.png");
	//assets->addTexture("projectile", "assets/proj.png");
	//assets->addTexture("scarecrow", "assets/scarecrow_animations.png");

	//assets->addFont("arial", "assets/arial.ttf", 16);

	//map = new Map("terrain", 3, 32);

	////ECS implementation

	//map->loadMap("assets/map.map", 25, 20);
	////map->loadMap("assets/ugly_collision_map2.txt", 20, 8);

	//player.addComponent<TransformComponent>(800, 640, 32, 32, 4);
	//player.addComponent<SpriteComponent>("player", true);
	//player.addComponent<KeyboardController>();
	//player.addComponent<ColliderComponent>("player");
	//player.addGroup(groupPlayers);
	//player.addComponent<HealthComponent>(3, "player");

	//scarecrow.addComponent<TransformComponent>(1000, 640, 32, 32, 4);
	//scarecrow.addComponent<SpriteComponent>("scarecrow", true);
	//scarecrow.addComponent<ColliderComponent>("scarecrow"); // <- just adding in a tag
	//scarecrow.addGroup(groupScarecrows);
	//scarecrow.addComponent<RandomWalkComponent>();
	//scarecrow.addComponent<HealthComponent>(1, "scarecrow");
	//enemyTimer = time(0);

	//SDL_Color white = { 255, 255, 255, 255 };
	//label.addComponent<UILabel>(10, 10, "Test String", "arial", white);

	////assets->createProjectile(Vector2D(600, 600), Vector2D(2,0), 200, 2, "projectile");
	////assets->createProjectile(Vector2D(600, 620), Vector2D(2, 0), 200, 2, "projectile");
	////assets->createProjectile(Vector2D(400, 600), Vector2D(2, 1), 200, 2, "projectile");
	////assets->createProjectile(Vector2D(600, 600), Vector2D(2, -1), 200, 2, "projectile");

	//enemiesSpawned = 1;
	//enemyLevel = 5;
	//enemiesLeftToKill = enemyLevel;



}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& scarecrows(manager.getGroup(Game::groupScarecrows)); // <- getting all the scarecrow entities from manager

void Game::update() 
{

	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Game::mPlayerPos = player.getComponent<TransformComponent>().position;
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


	camera.x = player.getComponent<TransformComponent>().position.x - 400;
	camera.y = player.getComponent<TransformComponent>().position.y - 320;

	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;

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
		gameOver.addComponent<UILabel>(400,320, "GAME OVER", "arial", orange);
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

void Game::handleEvents() {
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		this->isRunning = false;
		break;
	}
}




void Game::render(){
	SDL_RenderClear(this->renderer);
	// This is where we would add things to render
	//map->drawMap();
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
	//welcome.draw();
	label.draw();
	controls.draw();
	if (sandwichOn)
	{
		sandwich.draw();
		nextLevel.draw();
	}
	SDL_RenderPresent(this->renderer);
}
void Game::clean(){
	SDL_DestroyWindow(this->window);
	SDL_DestroyRenderer(this->renderer);
	SDL_Quit();
	std::cout << "Game Cleaned..." << std::endl;
}

void Game::flashWelcome()
{
	std::stringstream s;
	manager.refresh();
	manager.update();
	SDL_Color black = { 255, 165, 0, 255 };
	welcome.addComponent<UILabel>(10, 10, "Test String", "arial", black);
	SDL_RenderClear(this->renderer);
	welcome.draw();
	std::cout << "test string" << std::endl;
	time_t start = time(0);
	while (difftime(time(0), start) < 3)
	{ }


}
void Game::beginGame()
{
	assets->addTexture("terrain", "assets/terrain_ss.png");
	//assets->addTexture("terrain", "assets/ugly_ss.png");
	assets->addTexture("player", "assets/player_animations.png");
	assets->addTexture("projectile", "assets/water_bottle.png");
	assets->addTexture("scarecrow", "assets/scarecrow_animations.png");
	assets->addTexture("sandwich", "assets/sandwich_animation.png");

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

	//assets->createProjectile(Vector2D(600, 600), Vector2D(2,0), 200, 2, "projectile");
	//assets->createProjectile(Vector2D(600, 620), Vector2D(2, 0), 200, 2, "projectile");
	//assets->createProjectile(Vector2D(400, 600), Vector2D(2, 1), 200, 2, "projectile");
	//assets->createProjectile(Vector2D(600, 600), Vector2D(2, -1), 200, 2, "projectile");

	enemiesSpawned = 1;
	enemyLevel = 2;
	enemiesLeftToKill = enemyLevel;
	enemyTime = 4;

	sandwich.addComponent<TransformComponent>(-1, 320, 32, 32, 4);
	sandwich.addComponent<SpriteComponent>("sandwich", true, true);
	sandwich.addComponent<ColliderComponent>("sandwich");

}