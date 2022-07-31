#pragma once
#include "Component.h"

class Vector2;

class CircleCollisionComponent : public Component
{
public:
	CircleCollisionComponent(class Actor* ComponentOwner) : Component(ComponentOwner) { };

	float GetRadius() const { return mRadius; }
	void SetRadius(const float Radius) { mRadius = Radius; }

	Vector2 GetCenter() const;

	static bool Intersect(
		const CircleCollisionComponent& CircleA, const CircleCollisionComponent& CircleB);
	
private:
	float mRadius = 0.0f;
};

