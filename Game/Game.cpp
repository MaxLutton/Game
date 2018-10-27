#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"

Map* map;

//SDL_Renderer* Game::renderer = nullptr;

Manager manager;
auto& player(manager.addEntity());
auto& wall(manager.addEntity());
auto& tile0(manager.addEntity());
auto& tile1(manager.addEntity());
auto& tile2(manager.addEntity());

std::vector<ColliderComponent*> Game::colliders;

SDL_Event Game::event;

enum groupLabels : std::size_t
{
	groupMap,
	groupPlayers,
	groupEnemies,
	groupColliders
};


Game::Game() {

}

Game::~Game() {

}

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

	map = new Map();
	//ECS implementation
	Map::loadMap("assets/p16x16.map", 16, 16);

	player.addComponent<TransformComponent>(2);
	player.addComponent<SpriteComponent>("assets/stick_man.png");
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);

	wall.addComponent<TransformComponent>(300.0f, 300.0f, 300, 20, 1);
	wall.addComponent<SpriteComponent>("assets/dirt.png");
	wall.addComponent<ColliderComponent>("wall");
	wall.addGroup(groupMap);
}

void Game::update() {

	manager.refresh();
	manager.update();
	for (auto cc : colliders)
	{
		Collision::AABB(player.getComponent<ColliderComponent>(), *cc);
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

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));


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
	for (auto& e : enemies)
	{
		e->draw();
	}
	SDL_RenderPresent(this->renderer);
}
void Game::clean(){
	SDL_DestroyWindow(this->window);
	SDL_DestroyRenderer(this->renderer);
	SDL_Quit();
	std::cout << "Game Cleaned..." << std::endl;
}

void Game::addTile(int id, int x, int y)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(x, y, 32, 32, id);
	tile.addGroup(groupMap);

}