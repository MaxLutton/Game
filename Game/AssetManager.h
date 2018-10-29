#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS.h"

class AssetManager
{
public:
	AssetManager(Manager* man);
	~AssetManager();

	//Game objects
	void createProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id);

	//Texture Management
	void addTexture(std::string id, const char* path);
	SDL_Texture* getTexture(std::string id);
private:
	std::map<std::string, SDL_Texture*> textures;
	Manager* manager;
};