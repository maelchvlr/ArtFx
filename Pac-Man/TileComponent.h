#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include <SDL.h>

class TileComponent : public Component
{
public:
	TransformComponent* transform;
	SpriteComponent* sprite;
	ColliderComponent* collider;

	SDL_Rect tileRect;
	int tileID;
	const char* path;
	const char* tag;
	float scale;

	TileComponent() = default;

	TileComponent(int x, int y, int w, int h, int id)
	{
		tileRect.x = x;
		tileRect.y = y;
		tileRect.w = w;
		tileRect.h = h;
		tileID = id;

		switch (tileID)
		{
		case 0:
			path = "sprites/map/blank.png";
			tag = "Walls";
			scale = 0.9;
			break;
		case 1:
			path = "sprites/map/blank.png";
			tag = "Blank";
			break;
		case 2:
			path = "sprites/map/blank.png";
			tag = "Teleporter";
			scale = 0.2;
			break;
		case 3:
			path = "sprites/map/pill.png";
			tag = "Pills";
			scale = 0.2;
			break;
		case 4:
			path = "sprites/map/big-0.png";
			tag = "SuperPills";
			scale = 0.4;
			break;
		case 5:
			path = "sprites/map/pill.png";
			tag = "interpill";
			scale = 0.1;
			break;
		case 6:
			path = "sprites/map/blank.png";
			tag = "inter";
			scale = 0.1;
			break;
		default:
			break;
		}

	}

	void init() override
	{
		entity->addComponent<TransformComponent>((float) tileRect.x, (float) tileRect.y, tileRect.w, tileRect.h, 1);
		transform = &entity->getComponent<TransformComponent>();

		entity->addComponent<SpriteComponent>(path);
		sprite = &entity->getComponent<SpriteComponent>();

		entity->addComponent<ColliderComponent>(tag, scale);
		collider = &entity->getComponent<ColliderComponent>();
	}
};
