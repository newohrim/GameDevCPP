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

void TextUIData::SetText(const std::string& Text, SDL_Renderer* Renderer)
{
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
	SDL_RenderCopy(Renderer, m_TextTex, nullptr, &ScaledRect);

	SDL_SetRenderDrawColor(
		Renderer, CachedColor.r, CachedColor.g, CachedColor.b, CachedColor.a);
}
