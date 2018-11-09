#include "Game.h"
#include "Map.h"

Game* game = nullptr;


int main(int argc, char* argv[]) {

	// Keeping track of frame rate
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	int frameTime;

	game = new Game();
	game->init("Game Engine", 800, 640, false);
	//game->flashWelcome();
	game->beginGame();

	while (game->running()) {
		frameStart = SDL_GetTicks();
		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;
		// To limit frame rate
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}
	game->clean();
	return 0;
}


