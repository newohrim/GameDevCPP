#include "UIImageComponent.h"

#include "core/Math.h"
#include "core/Actor.h"

UIImageComponent::UIImageComponent(
	SDL_Texture* Texture, UIContainerActor* UIContainer, int DrawOrder) 
	: UIComponent(UIContainer, DrawOrder), m_Texture(Texture)
{
	if (Texture) 
	{
		int ImageWidth, ImageHeight;
		SDL_QueryTexture(Texture, nullptr, nullptr, &ImageWidth, &ImageHeight);
		SetRectDimension(ImageWidth, ImageHeight);
	}
}

void UIImageComponent::DrawUI(SDL_Renderer* Renderer)
{
	SDL_BlendMode CachedBlendMode;
	SDL_GetRenderDrawBlendMode(Renderer, &CachedBlendMode);

	const Vector2 ParentPos = mOwner->GetPosition();
	const SDL_Rect ImageRect = GetDrawRect();
	const SDL_Point Pivot = GetPivot();
	const SDL_Rect ScaledRect
	{
		ParentPos.x + ImageRect.x - Pivot.x,
		ParentPos.y + ImageRect.y - Pivot.y,
		ImageRect.w * GetRectScale(),
		ImageRect.h * GetRectScale()
	};
	if (m_ImageOpacity < 1.0f) 
	{
		SDL_SetRenderDrawBlendMode(Renderer, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	}
	SDL_SetTextureColorMod(m_Texture, m_ColorMultiplier.r, m_ColorMultiplier.g, m_ColorMultiplier.b);
	SDL_SetTextureAlphaMod(m_Texture, (uint8_t)(m_ImageOpacity * 255));
	SDL_RenderCopyEx(
		Renderer, 
		m_Texture, 
		nullptr, 
		&ScaledRect, 
		-Math::ToDegrees(GetRectAngle()),
		nullptr, 
		SDL_FLIP_NONE
	);

	SDL_SetRenderDrawBlendMode(Renderer, CachedBlendMode);
	SDL_SetTextureColorMod(m_Texture, 255, 255, 255);
}
