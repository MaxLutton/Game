#include "Game.h"
#include "Map.h"
#include <time.h>
#include "Scenes/GameScene.h"

Game* game = nullptr;
int Game::scene;
Scene* Game::currentScene;

int main(int argc, char* argv[]) {

	// Keeping track of frame rate
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	int frameTime;

	time_t start = time(0);
	bool inGame = false;

	game = new Game();
	game->init("Game Engine", 800, 640, false);
	//game->beginGame();

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

		if (inGame == false && difftime(time(0), start) > 2)
		{
			delete game->currentScene;
			game->currentScene = new GameScene();
			game->currentScene->init();
			inGame = true;
		}
	}
	game->clean();
	return 0;
}


