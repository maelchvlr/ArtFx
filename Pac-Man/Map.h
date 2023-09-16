#pragma once
#include "Game.h"

class Map
{
public:
	Map();
	~Map();

	void LoadMap();
	void Bg();


private:

	SDL_Rect src, dest;
	SDL_Texture* wholeMap;


};

