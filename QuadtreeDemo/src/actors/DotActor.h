#pragma once

#include "core/Actor.h"

class DotRenderComponent;
class Quadtree;

class DotActor : public Actor
{
public:
	DotActor(Quadtree* DotsTree, Game* GameInstance);

	virtual void UpdateActor(const float DeltaTime) override;

	DotRenderComponent* GetDotRenderComponent() const { return m_DotRenderer; }

private:
	DotRenderComponent* m_DotRenderer;

	Quadtree* m_DotsTree;

	Vector2 m_Direction;

	float m_MoveSpeed = 10.0f;
};

