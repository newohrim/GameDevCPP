#include "PlayerCharacter.h"

#include "AnimSpriteComponent.h"
#include "CharacterMoveComponent.h"
#include "BoxCollisionComponent.h"
#include "Game.h"

PlayerCharacter::PlayerCharacter(Game* GameInstance) : Actor(GameInstance)
{
	mSpriteComponent = new AnimSpriteComponent(this);
	InitSpriteComponent();

	const SDL_Rect CollBox = {-100.0f, -100.0f, 200.0f, 200.0f};
	mCollisionComponent = new BoxCollisionComponent(CollBox, this);

	mMoveComponent = new CharacterMoveComponent(this, mCollisionComponent);
	mMoveComponent->SetMoveSpeed(250.0f);
}

void PlayerCharacter::UpdateActor(const float DeltaTime)
{
	Actor::UpdateActor(DeltaTime);

	const Vector2 Velocity = mMoveComponent->GetVelocity();
	if (!Math::NearZero(Velocity.LengthSq())) 
	{
		Vector2 VeclocityDir = Velocity;
		VeclocityDir.Normalize();
		if (Vector2::Dot(VeclocityDir, Vector2(1.0f, 0.0f)) > 0.0f)
		{
			mSpriteComponent->FlipHorizontally(false);
			mSpriteComponent->SetCurrentAnimState("Running");
		}
		else 
		{
			mSpriteComponent->FlipHorizontally(true);
			mSpriteComponent->SetCurrentAnimState("Running");
		}
	}
	else 
	{
		mSpriteComponent->SetCurrentAnimState("Idle");
	}
}

void PlayerCharacter::MoveCharacter(Vector2 Direction, const float DeltaTime)
{
	const float HorizontalMovement = Vector2::Dot(Direction, Vector2(1.0f, 0.0f));
	if (HorizontalMovement > 0.0f) 
	{
		mSpriteComponent->SetCurrentAnimState("Running");
	}
	else 
	{
		mSpriteComponent->SetCurrentAnimState("Running");
	}

	Vector2 NewLocation = GetPosition() + Direction * mMoveSpeed * DeltaTime;
	SetPosition(NewLocation);
}

void PlayerCharacter::InitSpriteComponent()
{
	Game* GameInstance = GetGame();

	if (SDL_Texture* Texture = GameInstance->GetTexture(
		"Assets/PlayerCharacter/GraveRobber_idle.png"))
	{
		mSpriteComponent->AddAnimState("Idle", AnimationShot{ Texture, 48, 48, 0.5f });
	}

	if (SDL_Texture* Texture = GameInstance->GetTexture(
		"Assets/PlayerCharacter/GraveRobber_run.png"))
	{
		mSpriteComponent->AddAnimState("Running", AnimationShot{ Texture, 48, 48, 2.5f });
	}

	mSpriteComponent->SetCurrentAnimState("Idle");
	mSpriteComponent->SetAnimFPS(6.0f);
}
