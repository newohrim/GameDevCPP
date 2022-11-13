#include "CircleRigidbodyComponent.h"

#include "CircleCollisionComponent.h"
#include "Ball2D.h"
#include "Game.h"
#include <iostream>

void CircleRigidbodyComponent::Update(float DeltaTime)
{
    // Mass must be positive
    const Vector2 AccelerationVector = mSumOfForces * (1.0f / mMass);
    mVelocity += AccelerationVector * DeltaTime;
    mOwner->SetPosition(mOwner->GetPosition() + mVelocity * DeltaTime);

    mSumOfForces = Vector2::Zero;
}

void CircleRigidbodyComponent::AddForce(const Vector2 ForceVector)
{
    mSumOfForces += ForceVector;
}

void CircleRigidbodyComponent::AddImpulse(const Vector2 ForceVector)
{
    const Vector2 AccelerationVector = ForceVector * (1.0f / mMass);
    mVelocity += AccelerationVector;
}

void CircleRigidbodyComponent::BounceFrom(CircleRigidbodyComponent* A, CircleRigidbodyComponent* B, const Vector2 CollisionNormal)
{
    const Vector2 RelativeVelocity = B->GetRigidbodyVelocity() - A->GetRigidbodyVelocity();
    const float VelocityAlongNormal = Vector2::Dot(RelativeVelocity, CollisionNormal);

    if (VelocityAlongNormal > 0.0f)
        return;

    const float Restitution = Math::Min(A->GetRestitution(), B->GetRestitution());
    float ImpulseScalar = -(1 + Restitution) * VelocityAlongNormal;
    ImpulseScalar /= 1.0f / A->GetRigidbodyMass() + 1.0f / B->GetRigidbodyMass();

    const Vector2 ImpulseVector = ImpulseScalar * CollisionNormal;
    A->mVelocity -= 1.0f / A->GetRigidbodyMass() * ImpulseVector;
    B->mVelocity += 1.0f / B->GetRigidbodyMass() * ImpulseVector;
}
