#pragma once

#include "Actor.h"

class AnimSpriteComponent;
class CharacterMoveComponent;
class BoxCollisionComponent;

class PlayerCharacter : public Actor
{
public:
	PlayerCharacter(class Game* GameInstance);

	virtual void UpdateActor(const float DeltaTime) override;

	void MoveCharacter(Vector2 Direction, const float DeltaTime);

	void SetMoveSpeed(const float MoveSpeed) { mMoveSpeed = MoveSpeed; };

	AnimSpriteComponent* GetSpriteComponent() const { return mSpriteComponent; }

	CharacterMoveComponent* GetMoveComponent() const { return mMoveComponent; }

protected:
	AnimSpriteComponent* mSpriteComponent;

	CharacterMoveComponent* mMoveComponent;

	BoxCollisionComponent* mCollisionComponent;

	float mMoveSpeed = 1.0f;

	virtual void InitSpriteComponent();
};

