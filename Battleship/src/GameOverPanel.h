#pragma once

#include "ui/UIContainerActor.h"
#include "SDL_ttf.h"

class UIPanelComponent;
class UIButtonComponent;
enum PlayerEnum;

class GameOverPanel : public UIContainerActor
{
public:
	GameOverPanel(TTF_Font* Font, PlayerEnum Winner, Game* GameInstance);

private:
	UIPanelComponent* m_PanelComponent;

	void ResetButtonClick_Handle(UIButtonComponent* Button);
};

