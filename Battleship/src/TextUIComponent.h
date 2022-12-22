#pragma once

#include "ui/UIComponent.h"
#include "SDL/SDL.h"
#include "SDL_ttf.h"
#include <string>

class TextUIComponent : public UIComponent
{
public:
	TextUIComponent(TTF_Font* Font, UIContainerActor* ComponentOwner, int UpdateOrder = 100);

	TextUIComponent(TTF_Font* Font, Actor* ComponentOwner, int UpdateOrder = 100);

	void SetText(const std::string& Text);

	void SetTextColor(const SDL_Color& TextColor) { m_TextColor = TextColor; }

	virtual void DrawUI(SDL_Renderer* Renderer) override;

private:
	TTF_Font* m_Font;

	SDL_Texture* m_TextTex;

	SDL_Color m_TextColor;
};

