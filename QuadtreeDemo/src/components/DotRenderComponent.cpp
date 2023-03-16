#include "DotRenderComponent.h"

#include "core/Actor.h"

DotRenderComponent::DotRenderComponent(Actor* owner) 
	: Component(owner)
{
}

void DotRenderComponent::DrawDot(SDL_Renderer* Renderer)
{
	SDL_Color CachedColor; 
	SDL_GetRenderDrawColor(
		Renderer, &CachedColor.r, &CachedColor.g, &CachedColor.b, &CachedColor.a);

	SDL_SetRenderDrawColor(
		Renderer, m_DotColor.r, m_DotColor.g, m_DotColor.b, m_DotColor.a);

	const Vector2 OwnerPos = mOwner->GetPosition();
	SDL_RenderDrawPointF(Renderer, OwnerPos.x, OwnerPos.y);

	SDL_SetRenderDrawColor(
		Renderer, CachedColor.r, CachedColor.g, CachedColor.b, CachedColor.a);
}
