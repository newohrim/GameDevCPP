#include "TextUIData.h"

TextUIData::TextUIData(
	TTF_Font* TextFont, const std::string& Text, SDL_Renderer* Renderer) 
	: m_Font(TextFont)
{
	if (!Text.empty()) 
	{
		SetText(Text, Renderer);
	}
}

TextUIData& TextUIData::operator=(TextUIData&& Other)
{
	if (this != &Other)
	{
		if (m_TextTex)
		{
			SDL_DestroyTexture(m_TextTex);
		}
		m_Font = Other.m_Font;
		m_TextTex = Other.m_TextTex;
		Other.m_TextTex = nullptr;
		m_TextColor = Other.m_TextColor;
		m_TextWidth = Other.m_TextWidth;
		m_TextHeight = Other.m_TextHeight;
	}

	return *this;
}

TextUIData::~TextUIData()
{
	ClearTextTexture();
}

void TextUIData::SetText(const std::string& Text, SDL_Renderer* Renderer)
{
	ClearTextTexture();

	SDL_Surface* SurfaceMessage =
		TTF_RenderText_Solid(m_Font, Text.c_str(), m_TextColor);
	m_TextTex =
		SDL_CreateTextureFromSurface(Renderer, SurfaceMessage);
	SDL_QueryTexture(
		m_TextTex, nullptr, nullptr, &m_TextWidth, &m_TextHeight);
	SDL_FreeSurface(SurfaceMessage);
}

void TextUIData::DrawText(int X_Mid, int Y_Mid, SDL_Renderer* Renderer) const
{
	if (!m_TextTex)
		return;

	SDL_Color CachedColor;
	SDL_GetRenderDrawColor(
		Renderer, &CachedColor.r, &CachedColor.g, &CachedColor.b, &CachedColor.a);

	const SDL_Rect ScaledRect
	{
		X_Mid - m_TextWidth / 2,
		Y_Mid - m_TextHeight / 2,
		m_TextWidth,
		m_TextHeight
	};
	SDL_SetRenderDrawColor(
		Renderer, m_TextColor.r, m_TextColor.g, m_TextColor.b, m_TextColor.a);
	if (SDL_RenderCopy(Renderer, m_TextTex, nullptr, &ScaledRect) < 0) 
	{
		SDL_Log("%s", SDL_GetError());
	}

	SDL_SetRenderDrawColor(
		Renderer, CachedColor.r, CachedColor.g, CachedColor.b, CachedColor.a);
}

void TextUIData::ClearTextTexture()
{
	if (m_TextTex)
	{
		SDL_DestroyTexture(m_TextTex);
	}
}
