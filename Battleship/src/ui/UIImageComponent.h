#pragma once

#include "UIComponent.h"

class UIImageComponent : public UIComponent
{
public:
	UIImageComponent(
		SDL_Texture* Texture, UIContainerActor* UIContainer, int DrawOrder = 100);

	void SetImageOpacity(const float ImageOpacity) { m_ImageOpacity = ImageOpacity; }

	virtual void DrawUI(SDL_Renderer* Renderer) override;

private:
	SDL_Texture* m_Texture;

	float m_ImageOpacity = 1.0f;
};

