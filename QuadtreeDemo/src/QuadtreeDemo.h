#pragma once

#include "core/Game.h"
#include "Quadtree.h"
#include <memory>
#include "QuadtreeDrawer.h"
#include "SDL_ttf.h"

class DotActor;
class UIContainerActor;
class TextUIComponent;

class QuadtreeDemo : public Game
{
public:
	QuadtreeDemo() : Game(), m_DotsTree({0, 900, 0, 900})
	{
	}

	virtual void LoadData() override;

	virtual void ProcessInput() override;

	virtual void UpdateGame() override;

	virtual void DrawCustom(SDL_Renderer* Renderer) override;

private:
	static constexpr float REPRODUCE_COOLDOWN = 2.5f;
	static const size_t m_DotsCount = 1000;
	std::vector<DotActor*> m_Dots;
	Quadtree m_DotsTree;
	std::unique_ptr<QuadtreeDrawer> m_QuadtreeDrawer;
	UIContainerActor* m_StatusLabelContainer;
	TextUIComponent* m_MsLabel;
	TextUIComponent* m_CountLabel;
	TTF_Font* m_MainTextFont;

	std::vector<DotActor*> InitializeDots(size_t Count, SDL_Point Bounds);

	Vector2 GetRandomPosition(SDL_Point Bounds) const;
};

