#pragma once

#include "ui/UIComponent.h"
#include "SDL/SDL.h"
#include "SDL_ttf.h"
#include <string>

class TextUIComponent : public UIComponent
{
public:
	TextUIComponent(TTF_Font* Font, UIContainerActor* ComponentOwner, int DrawOrder = 100);

	TextUIComponent(TTF_Font* Font, Actor* ComponentOwner, int DrawOrder = 100);

	~TextUIComponent();

	void SetText(const std::string& Text);

	void SetTextColor(const SDL_Color& TextColor) { m_TextColor = TextColor; }

	virtual void DrawUI(SDL_Renderer* Renderer) override;

private:
	// Replace vars with TextUIData
	TTF_Font* m_Font;

	SDL_Texture* m_TextTex;

	SDL_Color m_TextColor;

	void ClearTextTexture();
};

