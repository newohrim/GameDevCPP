#pragma once

#include "core/Component.h"
#include "SDL/SDL.h"

class DotRenderComponent : public Component
{
public:
	DotRenderComponent(Actor* owner);

	void DrawDot(SDL_Renderer* Renderer);

	const SDL_Color& GetDotColor() const { return m_DotColor; }

	void SetDotColor(const SDL_Color& Color) { m_DotColor = Color; }

private:
	SDL_Color m_DotColor = { 255, 255, 255, 255 };
};

