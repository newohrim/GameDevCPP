#pragma once

#include "Actor.h"

class SpriteComponent;

class SpriteActor : public Actor
{
public:
	SpriteActor(Game* GameInstance);

	SpriteComponent* GetSpriteComponent() const { return m_SpriteComponent; }

private:
	SpriteComponent* m_SpriteComponent;
};

