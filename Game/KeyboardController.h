#pragma once
#include "Game.h"
#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"
#include "Scenes/GameScene.h"
class KeyboardController : public Component {
public:
	TransformComponent* transform;
	SpriteComponent* sprite;
	bool shoot = false;
	int shootDirection = 0; // 0 == up, 1 == left, 2 == right, 3 == down


	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override {
		if (Game::event.type == SDL_KEYDOWN) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_w:
				transform->velocity.y = -1;
				sprite->play("Walk");
				shootDirection = 0;
				break;
			case SDLK_a:
				transform->velocity.x = -1;
				sprite->play("Walk");
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
				shootDirection = 1;
				break;
			case SDLK_d:
				transform->velocity.x = +1;
				sprite->play("Walk");
				shootDirection = 2;
				break;
			case SDLK_s:
				transform->velocity.y = 1;
				sprite->play("Walk");
				shootDirection = 3;
				break;
			case SDLK_SPACE:
				shoot = true;
				break;
			default:
				break;
			}
		}
		if (Game::event.type == SDL_KEYUP) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_w:
				transform->velocity.y = 0;
				sprite->play("Idle");
				break;
			case SDLK_a:
				transform->velocity.x = 0;
				sprite->play("Idle");
				sprite->spriteFlip = SDL_FLIP_NONE;
				break;
			case SDLK_d:
				transform->velocity.x = 0;
				sprite->play("Idle");
				break;
			case SDLK_s:
				transform->velocity.y = 0;
				sprite->play("Idle");
				break;
			case SDLK_ESCAPE:
				Game::isRunning = false;
				break;
			case SDLK_SPACE:
				if (shoot)
				{
					switch (shootDirection)
					{
					case 0:
						Game::assets->createProjectile(Vector2D(GameScene::mPlayerPos.x+ 50, GameScene::mPlayerPos.y - 50), Vector2D(0, -2), 100, 2, "projectile");
						break;
					case 1:
						Game::assets->createProjectile(Vector2D(GameScene::mPlayerPos.x - 40, GameScene::mPlayerPos.y + 20), Vector2D(-2, 0), 100, 2, "projectile");
						break;
					case 2:
						Game::assets->createProjectile(Vector2D(GameScene::mPlayerPos.x + 130, GameScene::mPlayerPos.y + 20), Vector2D(2, 0), 100, 2, "projectile");
						break;
					case 3:
						Game::assets->createProjectile(Vector2D(GameScene::mPlayerPos.x + 50, GameScene::mPlayerPos.y + 130), Vector2D(0, +2), 100, 2, "projectile");
						break;
					}

					shoot = false;
				}
				break;
			default:
				break;
			}
		}
	}
};