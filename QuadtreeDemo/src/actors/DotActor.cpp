#include "DotActor.h"

#include "components/DotRenderComponent.h"
#include "SDL/SDL.h"
#include "core/Game.h"
#include "core/Random.h"
#include "Quadtree.h"

DotActor::DotActor(Quadtree* DotsTree, Game* GameInstance)
	: Actor(GameInstance),
	  m_DotsTree(DotsTree)
{
	m_DotRenderer = new DotRenderComponent(this);

	// Direction is set to random unit vector
	const float Angle = Random::GetFloatRange(0, Math::Pi * 2);
	m_Direction = Vector2 { Math::Cos(Angle), Math::Sin(Angle) };
}

void DotActor::UpdateActor(const float DeltaTime)
{
	Vector2 NewPos = GetPosition() + m_Direction * m_MoveSpeed * DeltaTime;
	m_DotsTree->MoveEntity(this, NewPos, GetPosition());
	SetPosition(NewPos);

	const SDL_Point Bounds = GetGame()->GetWindowSize();
	if (NewPos.x < 0 || NewPos.x > Bounds.x)
	{
		m_Direction.x *= -1;
	}
	if (NewPos.y < 0 || NewPos.y > Bounds.y)
	{
		m_Direction.y *= -1;
	}
}
