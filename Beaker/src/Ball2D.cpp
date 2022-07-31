#include "Ball2D.h"

#include "Game.h"
#include "SpriteComponent.h"
#include "MoveConstantComponent.h"
#include "CircleCollisionComponent.h"
#include "CircleRigidbodyComponent.h"

Ball2D::Ball2D(Game* GameInstance) : Actor(GameInstance)
{
	mSpriteComponent = new SpriteComponent(this);
	mMoveComponent = new MoveConstantComponent(this);
	mCircleComponent = new CircleCollisionComponent(this);
	mCircleRigidbody = new CircleRigidbodyComponent(this);
}

Ball2D::Ball2D(Game* GameInstance, SDL_Texture* BallTexture, float Radius) : Ball2D(GameInstance)
{
	mSpriteComponent->SetTexture(BallTexture);
	mCircleComponent->SetRadius(Radius);
}

void Ball2D::UpdateActor(float DeltaTime)
{
	// Update logo position
	//Vector2 NewPos = GetPosition() + Velocity * DeltaTime;
	//SetPosition(NewPos);

	const Vector2 NewPos = GetPosition();
	const float LogoWidth = mSpriteComponent->GetTexWidth() * GetScale();
	const float LogoHeight = mSpriteComponent->GetTexHeight() * GetScale();
	Vector2 Velocity = mCircleRigidbody->GetRigidbodyVelocity();

	if (NewPos.x <= (LogoWidth / 2) && Velocity.x < 0.0f)
	{
		Velocity.x *= -1.0f;
	}
	// Did the logo collide with the right wall?
	else if (NewPos.x >= (1024 - LogoWidth / 2) && Velocity.x > 0.0f)
	{
		Velocity.x *= -1.0f;
	}

	// Did the logo collide with the top wall?
	if (NewPos.y <= (LogoHeight / 2) && Velocity.y < 0.0f)
	{
		Velocity.y *= -1;
	}
	// Did the logo collide with the bottom wall?
	else if (NewPos.y >= (768 - LogoHeight / 2) && Velocity.y > 0.0f)
	{
		Velocity.y *= -1;
	}

	mCircleRigidbody->AddImpulse(Velocity - mCircleRigidbody->GetRigidbodyVelocity());
}
