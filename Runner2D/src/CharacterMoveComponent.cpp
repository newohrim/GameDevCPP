#include "CharacterMoveComponent.h"

#include "Actor.h"
#include "Game.h"
#include "BoxCollisionComponent.h"

CharacterMoveComponent::CharacterMoveComponent(Actor* Owner, BoxCollisionComponent* Collision, int UpdateOrder)
	: Component(Owner, UpdateOrder)
{
	mOwnersCollision = Collision;
	// UNDEF BEHAVIOUR TODO
	// mOwnersCollision = mOwner->GetComponent<BoxCollisionComponent>();
}

void CharacterMoveComponent::Update(float DeltaTime)
{
	//mVelocity += mAcceleration * DeltaTime;
	if (!Math::NearZero(mVelocity.LengthSq())) 
	{
		ProcessMovement(DeltaTime, false);
		mVelocity = Vector2::Zero;
	}
	else 
	{
		//mLastVelocity = Vector2::Zero;
		//mAcceleration = Vector2::Zero;
	}
	AddForce(Vector2(0.0f, 9.8f) * DeltaTime);
	mAcceleration += mForcesSum * (1.0f / mMass);
	mVelocity += mAcceleration * DeltaTime;
	if (mVelocity.LengthSq() > 0.0f)//!Math::NearZero(mVelocity.LengthSq(), 0.0000001f))
	{
		ProcessMovement(DeltaTime, true);
	}
	else
	{
		mLastVelocity = Vector2::Zero;
		//mAcceleration = Vector2::Zero;
	}
	mVelocity = Vector2::Zero;
	mForcesSum = Vector2::Zero;
}

void CharacterMoveComponent::SetVelocity(const Vector2 VelocityVector)
{
	mVelocity = VelocityVector;
}

void CharacterMoveComponent::AddVelocity(const Vector2 VelocityVector)
{
	mVelocity += VelocityVector;
}

void CharacterMoveComponent::AddForce(const Vector2 ForceVector)
{
	mForcesSum += ForceVector;
}

void CharacterMoveComponent::ProcessMovement(float DeltaTime, bool UpdateAcceleration)
{
	Vector2 DeltaMovement = mVelocity * mMoveSpeed * DeltaTime;
	if (mOwnersCollision)
	{
		SDL_Rect Original = mOwnersCollision->GetCollisionRect();
		SDL_Rect Moved = mOwnersCollision->GetCollisionRect();
		Moved.x += DeltaMovement.x;
		Moved.y += DeltaMovement.y;
		mOwnersCollision->SetCollisionRect(Moved);
		const std::vector<BoxCollisionComponent*> OverlappingCollisions =
			mOwner->GetGame()->GetOverlappingCollisions(mOwnersCollision);
		mOwnersCollision->SetCollisionRect(Original);
		if (OverlappingCollisions.size() > 0)
		{
			DeltaMovement =
				BoxCollisionComponent::CollisionResolution(mOwnersCollision, OverlappingCollisions[0], DeltaMovement);
		}
	}

	const Vector2 TargetPosition =
		mOwner->GetPosition() + DeltaMovement;
	mOwner->SetPosition(TargetPosition);
	mLastVelocity = DeltaMovement;
	if (UpdateAcceleration) 
	{
		mAcceleration.x = DeltaMovement.x / DeltaTime;
		mAcceleration.y = DeltaMovement.y / DeltaTime;
	}
}
