#include "ChaseCalculator.h"
#include "Collision.h"
#include "ColliderComponent.h"

Vector2 ChaseCalculator::Prediction(Vector2& ghost, Vector2& PacMan, int prediction)
{
	int x = ghost.x - PacMan.x;
	int y = ghost.y - PacMan.y;

	if (abs(x) > abs(y))
	{
		if (x > 0)
		{
			return Vector2(-1, 0);
		}
		else {
			return Vector2(1, 0);
		}
	}
	else {
		if (y > 0)
		{
			return Vector2(0, -1);
		}
		else {
			return Vector2(0, 1);
		}
	}
}