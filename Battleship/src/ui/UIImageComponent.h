#pragma once

#include "ui/UIComponent.h"

class UIImageComponent : public UIComponent
{
public:
	UIImageComponent(
		SDL_Texture* Texture, UIContainerActor* UIContainer, int DrawOrder = 100);

	const SDL_Color& GetColorMultipler() const { return m_ColorMultiplier; }

	void SetColorMultiplier(const SDL_Color& ColorMultipler) { m_ColorMultiplier = ColorMultipler; }

	float GetImageOpacity() const { return m_ImageOpacity; }

	void SetImageOpacity(const float ImageOpacity) { m_ImageOpacity = ImageOpacity; }

	virtual void DrawUI(SDL_Renderer* Renderer) override;

private:
	SDL_Texture* m_Texture = nullptr;

	SDL_Color m_ColorMultiplier = { 255, 255, 255, 255 };

	float m_ImageOpacity = 1.0f;
};

