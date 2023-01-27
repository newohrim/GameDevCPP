#include "UIPanelComponent.h"

#include "SDL/SDL.h"
#include "core/Math.h"
#include "core/Actor.h"

UIPanelComponent::UIPanelComponent(UIContainerActor* UIContrainer, int DrawOrder) 
	: UIComponent(UIContrainer, DrawOrder)
{
	m_PanelColor = { 255, 255, 255 };
	m_PanelOpacity = 1.0f;
}

void UIPanelComponent::DrawUI(SDL_Renderer* Renderer)
{
	SDL_Color CachedColor;
	uint8_t CachedAlpha;
	SDL_GetRenderDrawColor(Renderer, &CachedColor.r, &CachedColor.g, &CachedColor.b, &CachedAlpha);
	SDL_BlendMode CachedBlendMode;
	SDL_GetRenderDrawBlendMode(Renderer, &CachedBlendMode);

	const Vector2 ParentPos = mOwner->GetPosition();
	const SDL_Rect& PanelRect = GetDrawRect();
	const SDL_Rect TargetRect
	{
		ParentPos.x + PanelRect.x,
		ParentPos.y + PanelRect.y,
		PanelRect.w,
		PanelRect.h
	};
	if (m_PanelOpacity < 1.0f) 
	{
		SDL_SetRenderDrawBlendMode(Renderer, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	}
	const uint8_t TargetOpacity = (uint8_t)(m_PanelOpacity * 255);
	SDL_SetRenderDrawColor(Renderer, m_PanelColor.r, m_PanelColor.g, m_PanelColor.b, TargetOpacity);
	SDL_RenderFillRect(Renderer, &TargetRect);

	SDL_SetRenderDrawColor(Renderer, CachedColor.r, CachedColor.g, CachedColor.b, CachedAlpha);
	SDL_SetRenderDrawBlendMode(Renderer, CachedBlendMode);
}
