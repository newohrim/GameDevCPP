#pragma once

#include "UIImageComponent.h"
#include <functional>
#include <string>
#include <memory>
#include "TextUIData.h"

class UIButtonComponent;
class TextUIData;

namespace UIButton 
{
	struct Callback
	{
		std::function<void(UIButtonComponent*)> m_Callback;
	};

	struct TextInfo 
	{
		const std::string Text; 
		TTF_Font* Font;
	};
}

class UIButtonComponent : public UIImageComponent
{
public:
	UIButtonComponent(
		SDL_Texture* Texture, UIContainerActor* UIContainer, int DrawOrder = 100);

	UIButtonComponent(
		const UIButton::TextInfo& ButtonText, UIContainerActor* UIContainer, int DrawOrder = 100);

	template<class T>
	void SetOnClickHandler(T* Obj, void(T::* Handler)(UIButtonComponent*)) 
	{
		m_OnClickHandler.m_Callback = std::bind(Handler, Obj, this);
	}

	virtual bool ConsumeInput_MouseClick(Vector2 MousePos) override;

	virtual bool ConsumeInput_MouseOver(Vector2 MousePos) override;

	virtual void DrawUI(SDL_Renderer* Renderer) override;

private:
	UIButton::Callback m_OnClickHandler;

	std::unique_ptr<TextUIData> m_TextData = nullptr;

	bool m_IsMouseOver = false;

	bool m_DrawRect = false;
};

