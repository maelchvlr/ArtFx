#pragma once
#include <SDL.h>
#include <iostream>
#include "SDL_image.h"
#include <vector>

class ColliderComponent;

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	void render();
	void clean();

	bool running() { return isRunning; }

	static void AddTile(int id, int x, int y);
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static SDL_Event lastKeyDown;
	static bool isKeyboardActive;
	static std::vector<ColliderComponent*> colliders;

private:
	bool isRunning;
	SDL_Window* window;
	int count;

};

