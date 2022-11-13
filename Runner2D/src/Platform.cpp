#include "Platform.h"

#include "PlatformSpriteComponent.h"
#include "BoxCollisionComponent.h"
#include "Game.h"

Platform::Platform(Game* GameInstance, const int PlatformLength) : Actor(GameInstance)
{
	mSpriteComponent = new PlatformSpriteComponent(this, PlatformLength);
	SDL_Rect BoxRect = {-120, -24.0f, 240.0f, 48.0f};
	mCollisionComponent = new BoxCollisionComponent(BoxRect, this);
}
