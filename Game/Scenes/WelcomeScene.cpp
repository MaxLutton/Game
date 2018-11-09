#include "WelcomeScene.h"
#include "../ECS.h"
#include "../Map.h"
#include "../Components.h"
#include "../Collision.h"
#include <sstream>

extern Manager manager;
extern Map* map;

auto& welcomeTiles(manager.getGroup(WelcomeScene::groupMap));

void WelcomeScene::init()
{
	Game::scene = 0;

	map = new Map("welcome", 1, 32);
	map->loadMap("assets/welcome_map.txt", 25, 20);

}

void WelcomeScene::update()
{
	manager.refresh();
	manager.update();
}

void WelcomeScene::render()
{
	SDL_RenderClear(Game::renderer);

	for (auto& t : welcomeTiles)
	{
		t->draw();
	}

	SDL_RenderPresent(Game::renderer);
}