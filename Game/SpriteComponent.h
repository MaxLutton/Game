#pragma once
#include "Components.h"
#include "SDL.h"
#include "TextureManager.h"

class SpriteComponent : public Component {
private:
	TransformComponent* transform;
	SDL_Texture* tex;
	SDL_Rect srcRect, dstRect;

public:
	SpriteComponent() = default;
	SpriteComponent(const char* path) {
		setTex(path);
	}
	~SpriteComponent() {
		SDL_DestroyTexture(tex);
	}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	void update() override {
		dstRect.x = static_cast<int>(transform->position.x);
		dstRect.y = static_cast<int>(transform->position.y);
		dstRect.w = transform->width * transform->scale;
		dstRect.h = transform->height * transform->scale;
	}
	void draw() override{
		TextureManager::Draw(tex, srcRect, dstRect);
	}
	void setTex(const char* path) {
		tex = TextureManager::LoadTexture(path);
	}
};