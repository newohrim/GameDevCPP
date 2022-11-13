#pragma once
#include "Actor.h"

class SpriteComponent;
class MoveConstantComponent;
class CircleCollisionComponent;
class CircleRigidbodyComponent;

class Ball2D : public Actor
{
public:
	Ball2D(class Game* GameInstance);
	Ball2D(class Game* GameInstance, struct SDL_Texture* BallTexture, float Radius);

	virtual void UpdateActor(float DeltaTime) override;

	SpriteComponent* GetSpriteComponent() const { return mSpriteComponent; }
	CircleCollisionComponent* GetCircleCollision() const { return mCircleComponent; }
	CircleRigidbodyComponent* GetCircleRigidbody() const { return mCircleRigidbody; }

protected:
	SpriteComponent* mSpriteComponent;
	MoveConstantComponent* mMoveComponent;
	CircleCollisionComponent* mCircleComponent;
	CircleRigidbodyComponent* mCircleRigidbody;
};

