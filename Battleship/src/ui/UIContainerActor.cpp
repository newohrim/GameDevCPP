#include "UIContainerActor.h"

#include "ui/UIComponent.h"
#include "core/Game.h"

UIContainerActor::UIContainerActor(Game* GameInstance) 
	: Actor(GameInstance)
{
	GameInstance->AddUIItem(this);
}

UIContainerActor::~UIContainerActor()
{
	GetGame()->RemoveUIItem(this);
}

void UIContainerActor::AddUIComponent(UIComponent* UIComponent)
{
	// Find the insertion point in the sorted vector
	// (The first element with a higher draw order than me)
	const int DrawOrder = UIComponent->GetDrawOrder();
	auto Iter = m_UIComponents.begin();
	for (;
		Iter != m_UIComponents.end();
		++Iter)
	{
		if (DrawOrder < (*Iter)->GetDrawOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	m_UIComponents.insert(Iter, UIComponent);

	GetGame()->AddRect2D(UIComponent);
}

void UIContainerActor::RemoveUIComponent(UIComponent* UIComponent)
{
	// (We can't swap because it ruins ordering)
	auto iter = std::find(m_UIComponents.begin(), m_UIComponents.end(), UIComponent);
	if (iter != m_UIComponents.end()) 
	{
		m_UIComponents.erase(iter);
	}

	GetGame()->RemoveRect2D(UIComponent);
}

void UIContainerActor::DrawUI(SDL_Renderer* Renderer)
{
	for (UIComponent* UIComp : m_UIComponents) 
	{
		UIComp->DrawUI(Renderer);
	}
}

bool UIContainerActor::IsPointInside(Vector2 Point)
{
	/*for (UIComponent* Comp : m_UIComponents) 
	{
		if (Comp->IsPointInside(Point)) 
		{
			return true;
		}
	}

	return false;*/
}

bool UIContainerActor::ConsumeInput_MouseClick(Vector2_Int MousePos)
{
	/*bool WasUIInteracted = false;
	for (UIComponent* Comp : m_UIComponents) 
	{
		WasUIInteracted |= Comp->ConsumeInput_MouseClick(MousePos);
	}

	return WasUIInteracted;*/
}

bool UIContainerActor::ConsumeInput_MouseOver(Vector2_Int MousePos)
{
	/*bool WasUIInteracted = false;
	for (UIComponent* Comp : m_UIComponents)
	{
		WasUIInteracted |= Comp->ConsumeInput_MouseOver(MousePos);
	}

	return WasUIInteracted;*/
}
