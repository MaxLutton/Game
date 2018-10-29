#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "SDL.h"

class TileComponent : public Component {
public:

	SDL_Rect srcRect, dstRect;
	SDL_Texture* texture;
	Vector2D position;

	TileComponent() = default;
	~TileComponent()
	{
		SDL_DestroyTexture(texture);
	}

	TileComponent(int srcX, int srcY, int xPos, int yPos, int tsize, int tscale, const char* path)
	{
		texture = TextureManager::LoadTexture(path);

		position.x = xPos;
		position.y = yPos;

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = tsize;

		dstRect.x = xPos;
		dstRect.y = yPos;
		dstRect.w = dstRect.h = tsize * tscale;
	}

	void update() override
	{
		dstRect.x = position.x - Game::camera.x;
		dstRect.y = position.y - Game::camera.y;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, dstRect, SDL_FLIP_NONE);
	}

};