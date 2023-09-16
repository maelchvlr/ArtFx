#pragma once
#include "Components.h"
#include "Vector2.h"

class TransformComponent : public Component
{
private:
	int xpos;
	int ypos;


public:

	Vector2 position;
	Vector2 velocity;

	int speed = 2;
	int height = 20;
	int width = 20;
	float scale = 1.0;



	int x() { return xpos; }
	int y() { return ypos; }

	TransformComponent()
	{
		position.Zero();
	}

	TransformComponent(float sc)
	{
		position.Zero();
		scale = sc;
	}


	TransformComponent(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	TransformComponent(float x, float y, int h, int w, float sc)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}

	void init() override
	{
		velocity.Zero();
	}

	void update() override
	{
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}


};