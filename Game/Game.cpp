#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <sstream>

Map* map;

//SDL_Renderer* Game::renderer = nullptr;

Manager manager;
auto& player(manager.addEntity());
//auto& tile0(manager.addEntity());
//auto& tile1(manager.addEntity());
//auto& tile2(manager.addEntity());
auto& label(manager.addEntity());

SDL_Event Game::event;

SDL_Rect Game::camera = { 0, 0, 800, 640 };

AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;

Game::Game() {}

Game::~Game() {}

void Game::init(const char* title, int width, int height, bool fullscreen) {

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

	assets->addTexture("terrain", "assets/terrain_ss.png");
	assets->addTexture("player", "assets/player_animations.png");
	assets->addTexture("projectile", "assets/proj.png");

	assets->addFont("arial", "assets/arial.ttf", 16);

	map = new Map("terrain", 3, 32);
	
	//ECS implementation
	
	map->loadMap("assets/map.map", 25, 20);

	player.addComponent<TransformComponent>(800, 640, 32, 32, 4);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);
	SDL_Color white = { 255, 255, 255, 255 };
	label.addComponent<UILabel>(10, 10, "Test String", "arial", white);

	assets->createProjectile(Vector2D(600, 600), Vector2D(2,0), 200, 2, "projectile");
	assets->createProjectile(Vector2D(600, 620), Vector2D(2, 0), 200, 2, "projectile");
	assets->createProjectile(Vector2D(400, 600), Vector2D(2, 1), 200, 2, "projectile");
	assets->createProjectile(Vector2D(600, 600), Vector2D(2, -1), 200, 2, "projectile");

}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));

void Game::update() 
{
	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	std::stringstream ss;
	ss << "Player position: " << playerPos;
	label.getComponent<UILabel>().setLabelText(ss.str(), "arial");

	manager.refresh();
	manager.update();

	for (auto& c : colliders)
	{
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		// If collision, just put keep player at position
		if (Collision::AABB(cCol, playerCol))
		{
			std::cout << "hit" << std::endl;
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}

	for (auto& p : projectiles)
	{
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
							p->getComponent<ColliderComponent>().collider))
		{
			std::cout << "Hit Player" << std::endl;
			p->destroy();
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
	for (auto& c : colliders)
	{
		c->draw();
	}
	for (auto& p : projectiles)
	{
		p->draw();
	}

	label.draw();

	SDL_RenderPresent(this->renderer);
}
void Game::clean(){
	SDL_DestroyWindow(this->window);
	SDL_DestroyRenderer(this->renderer);
	SDL_Quit();
	std::cout << "Game Cleaned..." << std::endl;
}
