#include "GameObject.h"
#include "TextureManager.h"

SDL_Renderer* Game::renderer = nullptr;

GameObject::GameObject(const char* textureSheet, int x, int y) {
	objTexture = TextureManager::LoadTexture(textureSheet);
	xpos = x;
	ypos = y;
}

void GameObject::Update() {

	xpos++;
	ypos++;

	srcRect.h = 32;
	srcRect.w = 32;
	srcRect.x = 0;
	srcRect.y = 0;

	dstRect.x = xpos;
	dstRect.y = ypos;
	dstRect.w = srcRect.w * 2;
	dstRect.h = srcRect.h * 2;
}

void GameObject::Render() {
	SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &dstRect);

}