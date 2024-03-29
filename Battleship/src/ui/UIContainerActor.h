#pragma once

#include "ui/UIInterface.h"
#include "core/Actor.h"
#include <vector>

class UIComponent;

class UIContainerActor : public Actor, public UIInterface
{
public:
	UIContainerActor(Game* GameInstance);

	~UIContainerActor();

	void AddUIComponent(UIComponent* UIComponent);

	void RemoveUIComponent(UIComponent* UIComponent);

	virtual void DrawUI(SDL_Renderer* Renderer) override;

	virtual bool IsPointInside(Vector2 Point) override;

	virtual bool ConsumeInput_MouseClick(Vector2 MousePos) override;

	virtual bool ConsumeInput_MouseOver(Vector2 MousePos) override;

private:
	std::vector<UIComponent*> m_UIComponents;
};

