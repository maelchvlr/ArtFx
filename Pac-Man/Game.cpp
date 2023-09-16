#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS.h"
#include "Components.h"
#include "Vector2.h"
#include "Collision.h"
#include "ChaseCalculator.h"
#include "Entities.h"


Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
SDL_Event Game::lastKeyDown;
bool Game::isKeyboardActive = true;

std::vector<ColliderComponent*> Game::colliders;

//PacMan* Player = new PacMan(manager, EntityState::awaiting);
auto& Player(manager.addEntity());
auto& blinky(manager.addEntity());
auto& clyde(manager.addEntity());
auto& inky(manager.addEntity());
auto& pinky(manager.addEntity());

enum groupLabels : std::size_t
{
	groupMap,
	groupGhosts,
	groupPlayer,
	groupColliders
};




Game::Game(){}

Game::~Game(){}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "subsystems initialized!" << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		if (window)
		{
			std::cout << "Window Succesfully created !" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			std::cout << "Renderer Succesfully created !" << std::endl;
		}

		isRunning = true;

	} else {
		isRunning = false;
	}

	map = new Map();
	


	Player.addComponent<TransformComponent>(270, 460);
	Player.setPreviousPos(Player.getComponent<TransformComponent>().position);
	Player.setState(EntityState::awaiting);
	Player.addComponent<SpriteComponent>("sprites/pacman-right/1.png");
	Player.addComponent<KeyboardController>();
	Player.addComponent<ColliderComponent>("Player", 0.8);
	Player.addGroup(groupPlayer);

	blinky.addComponent<TransformComponent>(270, 220);
	blinky.setPreviousPos(blinky.getComponent<TransformComponent>().position);
	blinky.getComponent<TransformComponent>().velocity = Vector2(1, 0);
	blinky.addComponent<SpriteComponent>("sprites/ghosts/blinky.png");
	blinky.addComponent<ColliderComponent>("ghost", 0.8);
	blinky.addGroup(groupGhosts);

	/*
	clyde.addComponent<TransformComponent>(90, 50);
	clyde.addComponent<SpriteComponent>("sprites/ghosts/clyde.png");

	inky.addComponent<TransformComponent>(110, 50);
	inky.addComponent<SpriteComponent>("sprites/ghosts/inky.png");

	pinky.addComponent<TransformComponent>(50, 50);
	pinky.addComponent<SpriteComponent>("sprites/ghosts/pinky.png");
	*/

	map->LoadMap();
}

void Game::handleEvents()
{
	
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT :
		isRunning = false;
		break;

	default:
		break;
	}
}

void Game::update() 
{
		
	manager.refresh();
	manager.update();


	for (auto collider : colliders)
	{
		if (Collision::AABB(Player.getComponent<ColliderComponent>(), *collider))
		{
			if (collider->tag == "Walls")
			{

				Player.getComponent<TransformComponent>().velocity = Vector2(0, 0);
				Player.getComponent<TransformComponent>().position = Player.previousPosition();
				Player.setState(EntityState::awaiting);

				std::cout << "state :" << Player.getState() << std::endl;
				
				
			}

			

			if (collider->tag == "interpill")
			{
				//Game::isKeyboardActive = true;
				collider->entity->getComponent<SpriteComponent>().setTex("sprites/map/blank.png");
				collider->tag = "inter";
				
			}

			if (collider->tag == "inter")
			{
				//Game::isKeyboardActive = true;
			}

			if (collider->tag == "Pills")
			{
				collider->entity->destroy();
			}
		}

		if (Collision::AABB(blinky.getComponent<ColliderComponent>(), *collider))
		{
			if (collider->tag == "Walls")
			{
				blinky.getComponent<TransformComponent>().position = blinky.previousPosition();
				blinky.getComponent<TransformComponent>().velocity = ChaseCalculator::Prediction(blinky.getComponent<TransformComponent>().position, Player.getComponent<TransformComponent>().position, 1);
				//std::cout << "blinky hit wall" << std::endl;
				//std::cout << " velocity : " << blinky.getComponent<TransformComponent>().velocity << std::endl;
				
			}
		}
		
		
	}

	Player.setPreviousPos(Player.getComponent<TransformComponent>().position);
	blinky.setPreviousPos(blinky.getComponent<TransformComponent>().position);



	if (((((int)Player.getComponent<TransformComponent>().position.x) % 20 + ((int)Player.getComponent<TransformComponent>().position.y) % 20) % 20 == 0))
	{
		switch ((((int)Player.getComponent<TransformComponent>().position.x) % 3 + ((int)Player.getComponent<TransformComponent>().position.y) % 3) % 3)
		{
		case 0:
			Player.getComponent<SpriteComponent>().setTex("sprites/pacman-right/3.png");
			break;
		case 1:
			Player.getComponent<SpriteComponent>().setTex("sprites/pacman-right/2.png");
			break;
		case 2:
			Player.getComponent<SpriteComponent>().setTex("sprites/pacman-right/1.png");
			break;
		}
	}

}

auto& tiles(manager.getGroup(groupMap));
auto& pacmans(manager.getGroup(groupPlayer));
auto& ghosts(manager.getGroup(groupGhosts));

void Game::render()
{
	SDL_RenderClear(renderer);

	map->Bg();

	for (auto& t : tiles)
	{
		t->draw();
	}
	for (auto& p : pacmans)
	{
		p->draw();
	}
	for (auto& g : ghosts)
	{
		g->draw();
	}



	SDL_RenderPresent(renderer);

}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "game cleaned" << std::endl;
}

void Game::AddTile(int id, int x, int y)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(x, y, 20, 20, id);
	tile.addGroup(groupMap);

}