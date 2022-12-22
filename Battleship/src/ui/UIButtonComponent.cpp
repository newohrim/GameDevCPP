#include "UIButtonComponent.h"

UIButtonComponent::UIButtonComponent(
	SDL_Texture* Texture, UIContainerActor* UIContainer, int DrawOrder)
	: UIImageComponent(Texture, UIContainer, DrawOrder) 
{
	m_OnClickHandler.m_Callback = nullptr;
}

void UIButtonComponent::ConsumeInput_MouseClick(Vector2 MousePos)
{
	if (IsPointInside(MousePos) && m_OnClickHandler.m_Callback)
	{
		m_OnClickHandler.m_Callback(this);
	}
}
