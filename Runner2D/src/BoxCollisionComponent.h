#pragma once

#include "Component.h"
#include "SDL/SDL_rect.h"
#include "Math.h"

class Actor;

class BoxCollisionComponent : public Component
{
public:
	BoxCollisionComponent(const SDL_Rect& CollisionRect, Actor* Owner, int UpdateOrder = 100);

	const SDL_Rect& GetCollisionRect() const { return mCollisionRect; }

	SDL_Rect GetCollisionRect_World() const;

	void SetCollisionRect(const SDL_Rect& NewRect) { mCollisionRect = NewRect; }

	void SetCollisionRect_World(const SDL_Rect& NewRect);

	static Vector2 CollisionResolution(const BoxCollisionComponent* A, const BoxCollisionComponent* B, const Vector2 VelocityA);

	static bool AABB(const BoxCollisionComponent* A, const BoxCollisionComponent* B);

	static Vector2 CalculateAABBDistance(const BoxCollisionComponent* A, const BoxCollisionComponent* B);

private:
	SDL_Rect mCollisionRect;
};

