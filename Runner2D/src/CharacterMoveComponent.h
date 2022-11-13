#pragma once

#include "Component.h"
#include "IMoveComponent.h"
#include "Math.h"

class Actor;
class BoxCollisionComponent;

class CharacterMoveComponent : public Component, IMoveComponent
{
public:
	CharacterMoveComponent(Actor* Owner, BoxCollisionComponent* Collision = nullptr, int UpdateOrder = 100);

	virtual void Update(float DeltaTime) override;

	virtual Vector2 GetVelocity() const override 
	{
		if (!Math::NearZero(mLastVelocity.LengthSq())) 
		{
			return mLastVelocity;
		}

		return mVelocity;
	}

	void SetVelocity(const Vector2 VelocityVector);

	void AddVelocity(const Vector2 VelocityVector);

	void AddForce(const Vector2 ForceVector);

	float GetMoveSpeed() const { return mMoveSpeed; }

	void SetMoveSpeed(float MoveSpeed) { mMoveSpeed = MoveSpeed; }

protected:
	void ProcessMovement(float DeltaTime, bool UpdateAcceleration = true);

private:
	Vector2 mVelocity;

	Vector2 mLastVelocity;

	Vector2 mForcesSum;

	Vector2 mAcceleration;

	float mMass = 70.0f;

	BoxCollisionComponent* mOwnersCollision = nullptr;

	float mMoveSpeed = 1.0f;
};

