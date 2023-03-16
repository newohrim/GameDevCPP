#include "DotActor.h"

#include "components/DotRenderComponent.h"

DotActor::DotActor(Game* GameInstance) 
	: Actor(GameInstance)
{
	m_DotRenderer = new DotRenderComponent(this);
}
