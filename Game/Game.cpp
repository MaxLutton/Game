#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <sstream>
#include "Scenes/GameScene.h"
#include "Scenes/Scene.h"
#include "Scenes/WelcomeScene.h"

// for random num
#include <stdlib.h> 
#include <time.h> 


Manager manager;

Vector2D Game::mPlayerPos;

time_t Game::lastLostLife;
time_t Game::enemyTimer;
int Game::enemiesSpawned;
int Game::enemyLevel;
int Game::enemiesLeftToKill;
int Game::score = 0;
int Game::enemyTime;
SDL_Renderer* Game::renderer;


SDL_Event Game::event;

SDL_Rect Game::camera = { 0, 0, 800, 640 };

AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;

Game::Game() {}

Game::~Game() {}

void Game::init(const char* title, int width, int height, bool fullscreen) {

	//lastLostLife = time(0);
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
	assets->addTexture("terrain", "assets/terrain_ss.png");
	//assets->addTexture("terrain", "assets/ugly_ss.png");
	assets->addTexture("player", "assets/player_animations.png");
	assets->addTexture("projectile", "assets/water_bottle.png");
	assets->addTexture("scarecrow", "assets/scarecrow_animations.png");
	assets->addTexture("sandwich", "assets/sandwich_animation.png");

	assets->addTexture("welcome", "assets/welcome_ss.png");

	this->currentScene = new WelcomeScene();
	this->currentScene->init();

}


void Game::update()
{
	this->currentScene->update();
}

void Game::render()
{
	this->currentScene->render();
}

void Game::handleEvents() {
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		this->isRunning = false;
		break;
	}
}

void Game::clean(){
	SDL_DestroyWindow(this->window);
	SDL_DestroyRenderer(this->renderer);
	SDL_Quit();
	std::cout << "Game Cleaned..." << std::endl;
}

