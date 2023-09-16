#pragma once
#include "Game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{
public:
	TransformComponent* transform;
	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override
	{
		switch (entity->getState())
			{
			case EntityState::walking:
				buffer();
				break;
			case EntityState::awaiting:
				buffer();

				if( !bufferedInput.equal(Vector2(0,0)))
				{
					entity->setState(EntityState::walking);
					entity->getComponent<TransformComponent>().velocity = bufferedInput;
				}

				bufferedInput = Vector2(0, 0);
			}
		


	}

	bool buffer()
	{
		if (Game::event.type == SDL_KEYDOWN)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_s:
				bufferedInput = Vector2(0, 1);
				break;
			case SDLK_z:
				bufferedInput = Vector2(0, -1);
				break;
			case SDLK_q:
				bufferedInput = Vector2(-1, 0);
				break;
			case SDLK_d:
				bufferedInput = Vector2(1, 0);
				break;
			default:
				break;
			}
			return true;
		}
		return false;
	}

	Vector2 getInput()
	{
		return bufferedInput;
	}

private:
	Vector2 bufferedInput;

};

