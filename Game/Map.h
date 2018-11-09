#pragma once
#include "Game.h"
#include <string>
class Map {
public:
	Map(std::string tID, int mscale, int msize);
	~Map();

	void loadMap(std::string path, int sizeX, int sizeY);
	void loadMenu(std::string path, int sizeX, int sizeY);
	void addTile(int srcX, int srcY, int xPos, int yPos);

private:
	std::string texID;
	int mapScale;
	int tileSize;
	int scaledSize;


};