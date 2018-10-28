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
	static void addTile(int srcX, int srcY, int xPos, int yPos);

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static SDL_Rect camera;

	static std::vector<ColliderComponent*>colliders;
	static bool isRunning;
private:
	int count;
	SDL_Window* window;
};


