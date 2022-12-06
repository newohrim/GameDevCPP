#include "TextUIComponent.h"

#include "Actor.h"
#include "Game.h"

TextUIComponent::TextUIComponent(TTF_Font* Font, Actor* ComponentOwner, int UpdateOrder)
	: Component(ComponentOwner, UpdateOrder), m_Font(Font)
{
	m_TextTex = nullptr;
	m_TextRect = SDL_Rect{ 0, 0, 100, 100 };
	m_TextColor = SDL_Color{ 255, 255, 255 };
	m_TextScale = 2.0f;

	mOwner->GetGame()->AddTextComponent(this);
}

TextUIComponent::~TextUIComponent()
{
	mOwner->GetGame()->RemoveTextComponent(this);
}

void TextUIComponent::SetText(const std::string& Text)
{
	SDL_Surface* SurfaceMessage =
		TTF_RenderText_Solid(m_Font, Text.c_str(), m_TextColor);
	SDL_Renderer* Renderer = mOwner->GetGame()->GetRenderer();
	m_TextTex = 
		SDL_CreateTextureFromSurface(Renderer, SurfaceMessage);
	SDL_QueryTexture(
		m_TextTex, nullptr, nullptr, &m_TextRect.w, &m_TextRect.h);
	SDL_FreeSurface(SurfaceMessage);
}

void TextUIComponent::SetTextPosition(const int X, const int Y)
{
	m_TextRect.x = X;
	m_TextRect.y = Y;
}

void TextUIComponent::SetTextScale(float TextScale)
{
	m_TextScale = TextScale;
}

void TextUIComponent::DrawText(SDL_Renderer* Renderer)
{
	if (!m_TextTex)
		return;

	const SDL_Rect ScaledRect 
	{
		m_TextRect.x,
		m_TextRect.y,
		m_TextRect.w * m_TextScale,
		m_TextRect.h * m_TextScale
	};
	SDL_RenderCopy(Renderer, m_TextTex, nullptr, &ScaledRect);
}
