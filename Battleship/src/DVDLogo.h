#pragma once

#include "Actor.h"

class SpriteComponent;

class DVDLogo : public Actor
{
public:
	DVDLogo(Game* GameInstance);
	virtual void UpdateActor(float DeltaTime) override;

private:
	SpriteComponent* LogoSprite;

	Vector2 Velocity = Vector2(-200.0f, 235.0f);
	int LogoWidth = 256;
	int LogoHeight = 116;
};

