#pragma once

#include "ui/UIComponent.h"
#include "SDL/SDL.h"

class UIPanelComponent : public UIComponent
{
public:
	UIPanelComponent(UIContainerActor* UIContrainer, int DrawOrder = 100);

	void SetPanelColor(const SDL_Color& PanelColor) { m_PanelColor = PanelColor; }

	void SetPanelOpacity(const float Opacity) { m_PanelOpacity = Opacity; }

	virtual void DrawUI(SDL_Renderer* Renderer) override;

private:
	SDL_Color m_PanelColor;

	float m_PanelOpacity;
};

