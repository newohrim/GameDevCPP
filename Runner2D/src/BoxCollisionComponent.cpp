#include "BoxCollisionComponent.h"

#include "Actor.h"
#include "Game.h"
#include <iostream>

BoxCollisionComponent::BoxCollisionComponent(const SDL_Rect& CollisionRect, Actor* Owner, int UpdateOrder) 
	: Component(Owner, UpdateOrder)
{
	mCollisionRect = CollisionRect;
	Owner->GetGame()->AddCollision(this);
}

SDL_Rect BoxCollisionComponent::GetCollisionRect_World() const
{
	SDL_Rect WorldRect(mCollisionRect);
	WorldRect.x = mOwner->GetPosition().x + mCollisionRect.x;
	WorldRect.y = mOwner->GetPosition().y + mCollisionRect.y;
	
	return WorldRect;
}

void BoxCollisionComponent::SetCollisionRect_World(const SDL_Rect& NewRect)
{
	mCollisionRect = NewRect;
	mCollisionRect.x -= mOwner->GetPosition().x;
	mCollisionRect.y -= mOwner->GetPosition().y;
}

Vector2 BoxCollisionComponent::CollisionResolution(const BoxCollisionComponent* A, const BoxCollisionComponent* B, const Vector2 VelocityA)
{
	const Vector2 Distance = BoxCollisionComponent::CalculateAABBDistance(A, B);
	Vector2 TimeToCollide(0.0f, 0.0f);
	TimeToCollide.x = !Math::NearZero(VelocityA.x) ? Math::Abs(Distance.x / VelocityA.x) : 0.0f;
	TimeToCollide.y = !Math::NearZero(VelocityA.y) ? Math::Abs(Distance.y / VelocityA.y) : 0.0f;

	float ShortestTime = 0.0f;
	Vector2 DeltaMovement(0.0f, 0.0f);
	if (!Math::NearZero(VelocityA.x) && Math::NearZero(VelocityA.y)) 
	{
		ShortestTime = TimeToCollide.x;
		DeltaMovement.x = ShortestTime * VelocityA.x;
	}
	else if (Math::NearZero(VelocityA.x) && !Math::NearZero(VelocityA.y)) 
	{
		ShortestTime = TimeToCollide.y;
		DeltaMovement.y = ShortestTime * VelocityA.y;
	}
	else 
	{
		ShortestTime = Math::Min(Math::Abs(TimeToCollide.x), Math::Abs(TimeToCollide.y));
		DeltaMovement = ShortestTime * VelocityA;
	}

	const float Length = DeltaMovement.Length();
	std::cout << "cl: " << Length << '\n';

	return DeltaMovement;
}

bool BoxCollisionComponent::AABB(const BoxCollisionComponent* A, const BoxCollisionComponent* B)
{
	return SDL_HasIntersection(
		&A->GetCollisionRect_World(),
		&B->GetCollisionRect_World());
}

Vector2 BoxCollisionComponent::CalculateAABBDistance(const BoxCollisionComponent* A, const BoxCollisionComponent* B)
{
	Vector2 Distance;
	const SDL_Rect A_World = A->GetCollisionRect_World();
	const SDL_Rect B_World = B->GetCollisionRect_World();

	if (A_World.x < B_World.x)
	{
		Distance.x = B_World.x - (A_World.x + A_World.w);
	}
	else if (A_World.x > B_World.x) 
	{
		Distance.x = A_World.x - (B_World.x + B_World.w);
	}

	if (A_World.y < B_World.y) 
	{
		Distance.y = B_World.y - (A_World.y + A_World.h);
	}
	else if (A_World.y > B_World.y) 
	{
		Distance.y = A_World.y - (B_World.y + B_World.h);
	}

	return Distance;
}
