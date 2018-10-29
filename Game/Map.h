#pragma once
#include "Game.h"
#include <string>
class Map {
public:
	Map(const char* map_path, int mscale, int msize);
	~Map();

	void loadMap(std::string path, int sizeX, int sizeY);
	void addTile(int srcX, int srcY, int xPos, int yPos);

private:
	const char* map_path;
	int mapScale;
	int tileSize;
	int scaledSize;


};