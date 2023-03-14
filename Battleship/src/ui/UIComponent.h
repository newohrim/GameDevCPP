#pragma once

#include "ui/UIInterface.h"
#include "core/Component.h"
#include "SDL/SDL.h"

class UIContainerActor;

class UIComponent : public Component, public UIInterface
{
public:
	UIComponent(UIContainerActor* UIContrainer, int DrawOrder = 100);

	UIComponent(Actor* ComponentOwner, int DrawOrder = 100);

	~UIComponent();

	const SDL_Rect& GetDrawRect() const { return m_DrawRect; }

	void SetRectPosition(const int X, const int Y);

	void SetRectDimension(const int Width, const int Height);

	float GetRectAngle() const { return m_DrawAngle; }

	void SetRectAngle(const float Angle) { m_DrawAngle = Angle; }

	float GetRectScale() const { return m_DrawScale; }

	void SetRectScale(const float RectScale);

	int GetDrawOrder() const { return m_DrawOrder; }

	SDL_Point GetPivot() const { return m_Pivot; }

	void SetPivot(const SDL_Point& Pivot) { m_Pivot = Pivot; }

	void SetPivotInMiddle();

	virtual bool IsPointInside(Vector2 Point) override;

	virtual bool ConsumeInput_MouseClick(Vector2 MousePos) override { return false; }

	virtual bool ConsumeInput_MouseOver(Vector2 MousePos) override { return false; }

private:
	UIContainerActor* m_ParentContainer;

	SDL_Rect m_DrawRect;

	SDL_Point m_Pivot;

	float m_DrawAngle;

	float m_DrawScale;

	int m_DrawOrder;

};

