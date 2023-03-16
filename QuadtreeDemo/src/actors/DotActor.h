#pragma once

#include "core/Actor.h"

class DotRenderComponent;

class DotActor : public Actor
{
public:
	DotActor(Game* GameInstance);

	DotRenderComponent* GetDotRenderComponent() const { return m_DotRenderer; }

private:
	DotRenderComponent* m_DotRenderer;
};

