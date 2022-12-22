#pragma once

#include "UIInterface.h"
#include "../Actor.h"
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

	virtual void ConsumeInput_MouseClick(Vector2 MousePos) override;

	virtual void ConsumeInput_MouseOver(Vector2 MousePos) override;

private:
	std::vector<UIComponent*> m_UIComponents;
};

