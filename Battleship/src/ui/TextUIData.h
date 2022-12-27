#pragma once

#include "SDL/SDL.h"
#include "SDL_ttf.h"
#include <string>

class TextUIData
{
public:
	TextUIData() = default;

	TextUIData(
		TTF_Font* TextFont, const std::string& Text = "", SDL_Renderer* Renderer = nullptr);

	void SetText(const std::string& Text, SDL_Renderer* Renderer);

	void SetTextColor(const SDL_Color& TextColor) { m_TextColor = TextColor; }

	void DrawText(int X_Mid, int Y_Mid, SDL_Renderer* Renderer) const;

	void GetTextDimension(int& TextWidth, int& TextHeight) const 
	{
		TextWidth = m_TextWidth;
		TextHeight = m_TextHeight;
	}

private:
	TTF_Font* m_Font = nullptr;

	SDL_Texture* m_TextTex = nullptr;

	SDL_Color m_TextColor = { 255, 255, 255, 255 };

	int m_TextWidth = 100;

	int m_TextHeight = 100;
};

