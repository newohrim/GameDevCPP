#include "UIButtonComponent.h"

#include "../Actor.h"
#include "../Game.h"

using namespace UIButton;

UIButtonComponent::UIButtonComponent(
	SDL_Texture* Texture, UIContainerActor* UIContainer, int DrawOrder)
	: UIImageComponent(Texture, UIContainer, DrawOrder) 
{
	m_OnClickHandler.m_Callback = nullptr;
	m_DrawRect = false;
}

UIButtonComponent::UIButtonComponent(
	const TextInfo& ButtonText, UIContainerActor* UIContainer, int DrawOrder) 
	: UIImageComponent(nullptr, UIContainer, DrawOrder)
{
	m_OnClickHandler.m_Callback = nullptr;
	m_DrawRect = true;

	m_TextData = std::unique_ptr<TextUIData>(
		new TextUIData(ButtonText.Font, ButtonText.Text, mOwner->GetGame()->GetRenderer()));
	int TextWidth, TextHeight;
	m_TextData->GetTextDimension(TextWidth, TextHeight);
	SetRectDimension(TextWidth, TextHeight);
}

bool UIButtonComponent::ConsumeInput_MouseClick(Vector2 MousePos)
{
	if (IsPointInside(MousePos) && m_OnClickHandler.m_Callback)
	{
		m_OnClickHandler.m_Callback(this);
		return true;
	}

	return false;
}

bool UIButtonComponent::ConsumeInput_MouseOver(Vector2 MousePos)
{
	const bool IsMouseOver = IsPointInside(MousePos);
	// State transition: false -> true
	if (!m_IsMouseOver && IsMouseOver)
	{
		m_IsMouseOver = true;
		SetImageOpacity(0.5f);
	}
	// State transition: true -> false
	else if (m_IsMouseOver && !IsMouseOver)
	{
		m_IsMouseOver = false;
		SetImageOpacity(1.0f);
	}
	// Deffault
	else 
	{
		// Avoid redrawing every frame
		return IsMouseOver;
	}

	// If state changed request redraw
	mOwner->GetGame()->RequestRedraw();
	return IsMouseOver;
}

void UIButtonComponent::DrawUI(SDL_Renderer* Renderer)
{
	if (!m_DrawRect) 
	{
		UIImageComponent::DrawUI(Renderer);
	}
	else 
	{
		// TODO: rewrite draw rect shitcode
		const Vector2 ParentPos = mOwner->GetPosition();
		const SDL_Rect ButtonRect = GetDrawRect();
		const float DrawScale = GetRectScale();
		const SDL_Rect ScaledRect = { 
			ParentPos.x + ButtonRect.x,// - Rect.w * DrawScale,
			ParentPos.y + ButtonRect.y,// - Rect.h * DrawScale,
			ButtonRect.w * DrawScale,
			ButtonRect.h * DrawScale
		};
		SDL_SetRenderDrawColor(Renderer, 
			100, 100, 100, GetImageOpacity() * 255);
		if (GetImageOpacity() < 1.0f) 
		{
			SDL_SetRenderDrawBlendMode(Renderer, SDL_BlendMode::SDL_BLENDMODE_BLEND);
		}
		SDL_RenderFillRect(Renderer, &ScaledRect);
	}

	if (m_TextData) 
	{
		const Vector2 ParentPos = mOwner->GetPosition();
		const SDL_Rect& ButtonRect = GetDrawRect();
		m_TextData->DrawText(
			// Text consumes middle rect point
			ParentPos.x + ButtonRect.x + ButtonRect.w / 2, 
			ParentPos.y + ButtonRect.y + ButtonRect.h / 2, 
			Renderer
		);
	}
}
