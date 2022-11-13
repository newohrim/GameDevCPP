#pragma once
#include "Component.h"

#include "Math.h"

class CircleCollisionComponent;

class CircleRigidbodyComponent : public Component
{
public:
	CircleRigidbodyComponent(class Actor* ComponentOwner, int UpdateOrder = 9)
		: Component(ComponentOwner, UpdateOrder) {}

	virtual void Update(float DeltaTime) override;

	float GetRigidbodyMass() const { return mMass; }
	float GetRestitution() const { return mRestitution; }
	Vector2 GetRigidbodyVelocity() const { return mVelocity; }

	virtual void AddForce(const Vector2 ForceVector);
	virtual void AddImpulse(const Vector2 ForceVector);
	
	static void BounceFrom(CircleRigidbodyComponent* A, CircleRigidbodyComponent* B, const Vector2 CollisionNormal);

private:
	float mMass = 1.0f;
	float mRestitution = 1.0f;
	Vector2 mVelocity;
	Vector2 mSumOfForces;
};

