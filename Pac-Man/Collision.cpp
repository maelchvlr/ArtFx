#include "Collision.h"
#include "ColliderComponent.h"

bool Collision::AABB(const SDL_Rect& A, const SDL_Rect& B)
{

	bool ARightOfB = A.x > B.x + B.w;
	bool ALeftOfB = A.x + A.w < B.x;
	bool AAboveB = A.y + A.h < B.y;
	bool ABelowB = A.y > B.y + B.h;


	return !(ARightOfB || ALeftOfB || AAboveB || ABelowB);
}

bool Collision::AABB(const ColliderComponent& A, const ColliderComponent& B)
{
	if (AABB(A.collider, B.collider))
	{
		//std::cout << A.tag << "HIT: " << B.tag << std::endl;
		return true;
	}

	return false;
}