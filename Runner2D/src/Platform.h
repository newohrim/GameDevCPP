#pragma once

#include "Actor.h"

class PlatformSpriteComponent;
class BoxCollisionComponent;

class Platform : public Actor
{
public:
	Platform(class Game* GameInstance, const int PlatformLength);

protected:
	PlatformSpriteComponent* mSpriteComponent;

	BoxCollisionComponent* mCollisionComponent;
};

