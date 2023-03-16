#include "UIComponent.h"

#include "ui/UIContainerActor.h"
#include "core/Game.h"
#include "ui/UIPanelComponent.h"

UIComponent::UIComponent(UIContainerActor* UIContrainer, int DrawOrder)
	: Component(UIContrainer), m_ParentContainer(UIContrainer), m_DrawOrder(DrawOrder)
{
	m_DrawRect = { 0, 0, 100, 100 };
	m_DrawScale = 1.0f;

	m_ParentContainer->AddUIComponent(this);
}

UIComponent::UIComponent(Actor* ComponentOwner, int DrawOrder) 
	: Component(ComponentOwner), m_DrawOrder(DrawOrder)
{
	m_DrawRect = { 0, 0, 100, 100 };
	m_DrawScale = 1.0f;

	m_ParentContainer = nullptr;
	ComponentOwner->GetGame()->AddUIItem(this);
}

UIComponent::~UIComponent()
{
	if (m_ParentContainer) 
	{
		m_ParentContainer->RemoveUIComponent(this);
	}
	else 
	{
		mOwner->GetGame()->RemoveUIItem(this);
	}
}

void UIComponent::SetRectPosition(const int X, const int Y)
{
	m_DrawRect.x = X;
	m_DrawRect.y = Y;
}

void UIComponent::SetRectDimension(const int Width, const int Height)
{
	m_DrawRect.w = Width;
	m_DrawRect.h = Height;
}

void UIComponent::SetRectScale(const float RectScale)
{
	m_DrawScale = RectScale;
}

bool UIComponent::IsPointInside(Vector2 Point)
{
	const SDL_Point SPoint = { Point.x, Point.y };
	const Vector2 OwnerPosition = mOwner->GetPosition();
	const SDL_Rect Rect =
	{
		OwnerPosition.x + m_DrawRect.x,
		OwnerPosition.y + m_DrawRect.y,
		m_DrawRect.w * m_DrawScale,
		m_DrawRect.h * m_DrawScale
	};

	return SDL_PointInRect(&SPoint, &Rect);
}
