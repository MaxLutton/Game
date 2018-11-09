#pragma once
#include "Components.h"
#include "SDL.h"
#include "TextureManager.h"
#include "Animation.h"
#include <map>
#include "AssetManager.h"
class SpriteComponent : public Component {
private:
	TransformComponent* transform;
	SDL_Texture* tex;
	SDL_Rect srcRect, dstRect;

	bool animated = false;
	int frames = 0;
	int speed = 100;

public:

	int animIndex = 0;

	std::map<const char*, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(std::string id) {
		setTex(id);
	}
	SpriteComponent(std::string id, bool isAnimated) {
		animated = isAnimated;

		Animation idle = Animation(0, 3, 200);
		Animation walk = Animation(1, 8, 200);
		Animation explode = Animation(2, 5, 200);

		animations.emplace("Idle", idle);
		animations.emplace("Walk", walk);
		animations.emplace("Explode", explode);
		
		play("Idle");

		setTex(id);
	}
	SpriteComponent(std::string id, bool isAnimated, bool isSandwich) : animated(isAnimated)
	{
		Animation sandwich = Animation(0, 5, 100);
		animations.emplace("Sandwich", sandwich);
		play("Sandwich");
		setTex(id);
	}

	~SpriteComponent() {}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	void update() override {

		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		srcRect.y = animIndex * transform->height;

		dstRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
		dstRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
		dstRect.w = transform->width * transform->scale;
		dstRect.h = transform->height * transform->scale;
	}
	void draw() override{
		TextureManager::Draw(tex, srcRect, dstRect, spriteFlip);
	}
	void setTex(std::string id) {
		tex = Game::assets->getTexture(id);
	}

	void play(const char* animName)
	{
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
	}
};