#include "Map.h"
#include "TextureManager.h"
#include "Game.h"
#include <fstream>
#include "ECS.h"
#include "Components.h"
#include "Scenes/GameScene.h"

extern Manager manager;

Map::Map(std::string tID, int mscale, int msize) : texID(tID), mapScale(mscale), tileSize(msize) {
	scaledSize = mscale * msize;
}

Map::~Map() {

}

void Map::loadMap(std::string path, int sizeX, int sizeY) 
{
	char c;
	std::fstream mapFile;
	mapFile.open(path);

	int srcX, srcY;

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(c);
			srcY = atoi(&c) * tileSize;
			mapFile.get(c);
			srcX = atoi(&c) * tileSize;
			addTile(srcX, srcY, x*scaledSize, y*scaledSize);
			mapFile.ignore();
		}
	}
	mapFile.ignore();
	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapFile.get(c);
			if (c == '1')
			{
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
				tcol.addGroup(GameScene::groupColliders);
			}
			mapFile.ignore();
		}
	}
	mapFile.close();
}

void Map::addTile(int srcX, int srcY, int xPos, int yPos)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xPos, yPos, tileSize, mapScale, texID);
	tile.addGroup(GameScene::groupMap);

}
