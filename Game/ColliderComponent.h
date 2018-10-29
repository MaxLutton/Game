#pragma once
#include <string>
#include "SDL.h"
#include "Components.h"
#include "TextureManager.h"

class ColliderComponent : public Component {
public:
	SDL_Rect collider;
	std::string tag;

	SDL_Texture* tex;
	SDL_Rect srcR, dstR;


	TransformComponent* transform;
	ColliderComponent(std::string t) { tag = t; }
	ColliderComponent(std::string t, int xPos, int yPos, int size)
	{
		tag = t;
		collider.x = xPos;
		collider.y = yPos;
		collider.h = collider.w = size;
	}
	void init() override {
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		tex = TextureManager::LoadTexture("assets/coltex.png");
		srcR = { 0,0,32,32 };
		dstR = { collider.x, collider.y, collider.w, collider.h };

	}

	void update() override 
	{
		if (tag != "terrain")
		{
			collider.x = static_cast<int>(transform->position.x);
			collider.y = static_cast<int>(transform->position.y);
			collider.w = transform->width * transform->scale;
			collider.h = transform->height * transform->scale;
		}

		dstR.x = collider.x - Game::camera.x;
		dstR.y = collider.y - Game::camera.y;
	}

	void draw() override
	{
		TextureManager::Draw(tex, srcR, dstR, SDL_FLIP_NONE);
	}
};