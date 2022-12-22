#pragma once

#include "UIInterface.h"
#include "../Component.h"
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

	float GetRectScale() const { return m_DrawScale; }

	void SetRectScale(const float RectScale);

	int GetDrawOrder() const { return m_DrawOrder; }

	virtual bool IsPointInside(Vector2 Point) override;

	virtual void ConsumeInput_MouseClick(Vector2 MousePos) override {  }

	virtual void ConsumeInput_MouseOver(Vector2 MousePos) override {  }

private:
	UIContainerActor* m_ParentContainer;

	SDL_Rect m_DrawRect;

	float m_DrawScale;

	int m_DrawOrder;
};
