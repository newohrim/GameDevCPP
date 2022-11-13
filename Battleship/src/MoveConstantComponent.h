#pragma once
#include "Component.h"

class Actor;

class MoveConstantComponent : public Component
{
public:
	MoveConstantComponent(Actor* ComponentOwner, int UpdateOrder = 10) 
		: Component(ComponentOwner, UpdateOrder) {};

	virtual void Update(float DeltaTime) override;

	float GetAngularSpeed() const { return mAngularSpeed; }
	void SetAngularSpeed(const float AngularSpeed) { mAngularSpeed = AngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	void SetForwardSpeed(const float ForwardSpeed) { mForwardSpeed = ForwardSpeed; }

private:
	float mAngularSpeed = 0.0f;
	float mForwardSpeed = 0.0f;
};

