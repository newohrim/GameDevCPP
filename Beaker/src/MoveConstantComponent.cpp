#include "MoveConstantComponent.h"
#include "Actor.h"
#include "Math.h"

void MoveConstantComponent::Update(float DeltaTime)
{
	if (!Math::NearZero(mAngularSpeed)) 
	{
		const float Rotation = 
			mOwner->GetRotation() + mAngularSpeed * DeltaTime;
		mOwner->SetRotation(Rotation);
	}
	if (!Math::NearZero(mForwardSpeed)) 
	{
		const Vector2 Position = 
			mOwner->GetPosition() + mOwner->GetForwardVector() * mForwardSpeed * DeltaTime;
		mOwner->SetPosition(Position);
	}
}
