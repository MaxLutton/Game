#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>

#include <iostream>

class ColliderComponent;

class Game {
public:
	Game();
	~Game();

	void init(const char* title, int width, int height, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void clean();
	bool running() { return isRunning; }
	static void addTile(int id, int x, int y);

	static SDL_Renderer* renderer;
	static SDL_Event event;

	static std::vector<ColliderComponent*>colliders;

private:
	int count;
	bool isRunning;
	SDL_Window* window;
};


