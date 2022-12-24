#include "UIButtonComponent.h"

#include "../Actor.h"
#include "../Game.h"

UIButtonComponent::UIButtonComponent(
	SDL_Texture* Texture, UIContainerActor* UIContainer, int DrawOrder)
	: UIImageComponent(Texture, UIContainer, DrawOrder) 
{
	m_OnClickHandler.m_Callback = nullptr;
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
	if (!m_IsMouseOver && IsMouseOver)
	{
		m_IsMouseOver = true;
		SetImageOpacity(0.5f);
	}
	else if (m_IsMouseOver && !IsMouseOver)
	{
		m_IsMouseOver = false;
		SetImageOpacity(1.0f);
	}
	else 
	{
		// Avoid redrawing every frame
		return IsMouseOver;
	}

	mOwner->GetGame()->RequestRedraw();
	return IsMouseOver;
}
