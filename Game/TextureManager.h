#pragma once

#include "Game.h"

class TextureManager {
public:
	static SDL_Texture* LoadTexture(const char* texture);
	static void Draw(SDL_Texture* tex, SDL_Rect srcRect, SDL_Rect dstRect, SDL_RendererFlip flip);
};

