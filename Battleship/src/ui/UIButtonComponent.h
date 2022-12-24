#pragma once

#include "UIImageComponent.h"
#include <functional>

class UIButtonComponent;

namespace UIButton 
{
	struct Callback
	{
		std::function<void(UIButtonComponent*)> m_Callback;
	};
}

class UIButtonComponent : public UIImageComponent
{
public:
	UIButtonComponent(
		SDL_Texture* Texture, UIContainerActor* UIContainer, int DrawOrder = 100);

	template<class T>
	void SetOnClickHandler(T* Obj, void(T::* Handler)(UIButtonComponent*)) 
	{
		m_OnClickHandler.m_Callback = std::bind(Handler, Obj, this);
	}

	virtual bool ConsumeInput_MouseClick(Vector2 MousePos) override;

	virtual bool ConsumeInput_MouseOver(Vector2 MousePos) override;

private:
	UIButton::Callback m_OnClickHandler;

	bool m_IsMouseOver = false;
};

