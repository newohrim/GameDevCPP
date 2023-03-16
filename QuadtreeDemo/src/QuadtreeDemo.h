#pragma once

#include "core/Game.h"
#include "Quadtree.h"
#include <memory>
#include "QuadtreeDrawer.h"

class DotActor;

class QuadtreeDemo : public Game
{
public:
	QuadtreeDemo() : Game(), m_DotsTree({0, 900, 0, 900})
	{
	}

	virtual void LoadData() override;

	virtual void ProcessInput() override;

	virtual void DrawCustom(SDL_Renderer* Renderer) override;

private:
	static const size_t m_DotsCount = 100;
	std::vector<DotActor*> m_Dots;
	Quadtree m_DotsTree;
	std::unique_ptr<QuadtreeDrawer> m_QuadtreeDrawer;

	std::vector<DotActor*> InitializeDots(size_t Count, SDL_Point Bounds);

	Vector2 GetRandomPosition(SDL_Point Bounds) const;
};
