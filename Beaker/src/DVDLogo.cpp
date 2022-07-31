#include "DVDLogo.h"
#include "SpriteComponent.h"
#include "Game.h"

DVDLogo::DVDLogo(Game* GameInstance) : Actor(GameInstance)
{
	LogoSprite = new SpriteComponent(this);
	LogoSprite->SetTexture(GameInstance->GetTexture("dvd_logo_yellow.png"));
}

void DVDLogo::UpdateActor(float DeltaTime)
{
	// Update logo position
	Vector2 NewPos = GetPosition() + Velocity * DeltaTime;
	SetPosition(NewPos);

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
}
