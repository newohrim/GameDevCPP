#include "UIImageComponent.h"

#include "../Math.h"
#include "../Actor.h"

UIImageComponent::UIImageComponent(
	SDL_Texture* Texture, UIContainerActor* UIContainer, int DrawOrder) 
	: UIComponent(UIContainer, DrawOrder), m_Texture(Texture)
{
	int ImageWidth, ImageHeight;
	SDL_QueryTexture(Texture, nullptr, nullptr, &ImageWidth, &ImageHeight);
	SetRectDimension(ImageWidth, ImageHeight);
}

void UIImageComponent::DrawUI(SDL_Renderer* Renderer)
{
	SDL_BlendMode CachedBlendMode;
	SDL_GetRenderDrawBlendMode(Renderer, &CachedBlendMode);

	const Vector2 ParentPos = mOwner->GetPosition();
	const SDL_Rect ImageRect = GetDrawRect();
	const SDL_Rect ScaledRect
	{
		ParentPos.x + ImageRect.x,
		ParentPos.y + ImageRect.y,
		ImageRect.w * GetRectScale(),
		ImageRect.h * GetRectScale()
	};
	if (m_ImageOpacity < 1.0f) 
	{
		SDL_SetRenderDrawBlendMode(Renderer, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	}
	SDL_SetTextureAlphaMod(m_Texture, (uint8_t)(m_ImageOpacity * 255));
	SDL_RenderCopy(Renderer, m_Texture, nullptr, &ScaledRect);

	SDL_SetRenderDrawBlendMode(Renderer, CachedBlendMode);
}
