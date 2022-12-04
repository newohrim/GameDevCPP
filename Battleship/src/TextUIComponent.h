#pragma once

#include "Component.h"
#include "SDL/SDL.h"
#include "SDL_ttf.h"
#include <string>

class TextUIComponent : public Component
{
public:
	TextUIComponent(TTF_Font* Font, class Actor* ComponentOwner, int UpdateOrder = 100);

	~TextUIComponent();

	void SetText(const std::string& Text);

	void SetTextPosition(const int X, const int Y);

	void SetTextScale(float TextScale);

	void SetTextColor(const SDL_Color& TextColor) { m_TextColor = TextColor; }

	void DrawText(SDL_Renderer* Renderer);

private:
	TTF_Font* m_Font;

	SDL_Texture* m_TextTex;

	SDL_Rect m_TextRect;
	
	SDL_Color m_TextColor;

	float m_TextScale;
};

