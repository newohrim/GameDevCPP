#include "TextUIComponent.h"

#include "core/Actor.h"
#include "core/Game.h"

TextUIComponent::TextUIComponent(TTF_Font* Font, UIContainerActor* ComponentOwner, int DrawOrder)
	:UIComponent(ComponentOwner, DrawOrder), m_Font(Font)
{
	m_TextTex = nullptr;
	m_TextColor = SDL_Color{ 255, 255, 255 };
}

TextUIComponent::TextUIComponent(TTF_Font* Font, Actor* ComponentOwner, int DrawOrder)
	: UIComponent(ComponentOwner, DrawOrder), m_Font(Font)
{
	m_TextTex = nullptr;
	m_TextColor = SDL_Color{ 255, 255, 255 };
}

TextUIComponent::~TextUIComponent()
{
	ClearTextTexture();
}

void TextUIComponent::SetText(const std::string& Text)
{
	ClearTextTexture();

	SDL_Surface* SurfaceMessage =
		TTF_RenderText_Solid(m_Font, Text.c_str(), m_TextColor);
	SDL_Renderer* Renderer = mOwner->GetGame()->GetRenderer();
	m_TextTex = 
		SDL_CreateTextureFromSurface(Renderer, SurfaceMessage);
	int TexWidth, TexHeight;
	SDL_QueryTexture(
		m_TextTex, nullptr, nullptr, &TexWidth, &TexHeight);
	SetRectDimension(TexWidth, TexHeight);
	SDL_FreeSurface(SurfaceMessage);
}

void TextUIComponent::DrawUI(SDL_Renderer* Renderer)
{
	if (!m_TextTex)
		return;

	const Vector2 ParentPos = mOwner->GetPosition();
	const SDL_Rect TextRect = GetDrawRect();
	const SDL_Rect ScaledRect 
	{
		ParentPos.x + TextRect.x,
		ParentPos.y + TextRect.y,
		TextRect.w * GetRectScale(),
		TextRect.h * GetRectScale()
	};
	SDL_RenderCopy(Renderer, m_TextTex, nullptr, &ScaledRect);
}

void TextUIComponent::ClearTextTexture()
{
	if (m_TextTex)
	{
		SDL_DestroyTexture(m_TextTex);
	}
}
