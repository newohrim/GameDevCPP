#include "SpriteActor.h"

#include "SpriteComponent.h"

SpriteActor::SpriteActor(Game* GameInstance) : Actor(GameInstance)
{
	m_SpriteComponent = new SpriteComponent(this);
}
