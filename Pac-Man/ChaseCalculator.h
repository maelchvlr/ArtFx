#pragma once
#include "Components.h"
#include "Vector2.h"
#include <math.h>

class Entity;

class ChaseCalculator
{
public:

	static Vector2 Prediction(Vector2& ghost, Vector2& PacMan, int prediction);
	

};

