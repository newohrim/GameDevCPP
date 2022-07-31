#include "CircleCollisionComponent.h"

#include "Actor.h"
#include "Math.h"

Vector2 CircleCollisionComponent::GetCenter() const
{
	return Vector2(
		mOwner->GetPosition().x + mRadius, 
		mOwner->GetPosition().y + mRadius);
}

bool CircleCollisionComponent::Intersect(const CircleCollisionComponent& CircleA, const CircleCollisionComponent& CircleB)
{
	const Vector2 Diff = CircleA.GetCenter() - CircleB.GetCenter();
	const float SqrDistance = Diff.LengthSq();
	const float RadiusSum = CircleA.GetRadius() + CircleB.GetRadius();

	return SqrDistance <= (RadiusSum * RadiusSum);
}
