#include "UIComponent.h"

#include "ui/UIContainerActor.h"
#include "core/Game.h"
#include "ui/UIPanelComponent.h"

UIComponent::UIComponent(UIContainerActor* UIContrainer, int DrawOrder)
	: Component(UIContrainer), m_ParentContainer(UIContrainer), m_DrawOrder(DrawOrder)
{
	m_DrawAngle = 0.0f;
	m_DrawRect = { 0, 0, 100, 100 };
	m_Pivot = { 0, 0 };
	m_DrawScale = 1.0f;

	m_ParentContainer->AddUIComponent(this);
}

UIComponent::UIComponent(Actor* ComponentOwner, int DrawOrder) 
	: Component(ComponentOwner), m_DrawOrder(DrawOrder)
{
	m_DrawAngle = 0.0f;
	m_DrawRect = { 0, 0, 100, 100 };
	m_Pivot = { 0, 0 };
	m_DrawScale = 1.0f;

	m_ParentContainer = nullptr;
	ComponentOwner->GetGame()->AddUIItem(this);
	ComponentOwner->GetGame()->AddRect2D(this);
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
		mOwner->GetGame()->RemoveRect2D(this);
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

void UIComponent::SetPivotInMiddle()
{
	m_Pivot = { m_DrawRect.w / 2, m_DrawRect.h / 2 };
}

bool UIComponent::IsPointInside(Vector2 Point)
{
	const Vector2 Temp = mOwner->GetPosition() + Vector2{ (float)m_DrawRect.x, (float)m_DrawRect.y };// - Vector2{ (float)m_Pivot.x, (float)m_Pivot.y };
	const Vector2 Dir = Point - Temp;
	//const Vector2 WorldToLocalOffset =
	//	mOwner->GetPosition() * (-1); // -
		//Vector2 { m_DrawRect.x, m_DrawRect.y } -
		//Vector2{ m_Pivot.x, m_Pivot.y };
	const Vector2 RotatedPoint = Vector2::Transform(
		Dir, // for non-static screen Point must be converted to world coordinates
		Matrix3::CreateRotation(-GetRectAngle())
	) + Temp;
	const SDL_Point SPoint = { RotatedPoint.x, RotatedPoint.y };
	const SDL_Rect Rect =
	{
		mOwner->GetPosition().x + m_DrawRect.x - m_Pivot.x,
		mOwner->GetPosition().y + m_DrawRect.y - m_Pivot.y,
		m_DrawRect.w * m_DrawScale,
		m_DrawRect.h * m_DrawScale
	};

	return SDL_PointInRect(&SPoint, &Rect);
}
