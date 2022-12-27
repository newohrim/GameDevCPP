#include "GameOverPanel.h"

#include "ui/UIPanelComponent.h"
#include "ui/UIButtonComponent.h"
#include "ui/TextUIData.h"
#include "TextUIComponent.h"
#include "Game.h"

GameOverPanel::GameOverPanel(TTF_Font* Font, PlayerEnum Winner, Game* GameInstance)
	: UIContainerActor(GameInstance)
{
	{
		m_PanelComponent = new UIPanelComponent(this, -1);
		m_PanelComponent->SetPanelColor({ 125, 125, 125 });
		const SDL_Rect PanelRect = { 0, 0, 400, 200 };
		m_PanelComponent->SetRectPosition(PanelRect.x, PanelRect.y);
		m_PanelComponent->SetRectDimension(PanelRect.w, PanelRect.h);
	}

	{
		TextUIComponent* const TextComponent = 
			new TextUIComponent(Font, this, 1);
		TextComponent->SetText(
			Winner == PlayerEnum::Player ? "You won!" : "You lost!");
		const SDL_Rect& PanelRect = m_PanelComponent->GetDrawRect();
		const SDL_Rect& TextRect = TextComponent->GetDrawRect();
		TextComponent->SetRectPosition(
			PanelRect.w / 2 - TextRect.w / 2,
			PanelRect.h / 2 - TextRect.h / 2
		);
	}
	
	{
		const UIButton::TextInfo ButtonText = { "Play again", Font };
		UIButtonComponent* const ResetButton = 
			new UIButtonComponent(ButtonText, this, 1);
		const SDL_Rect& ButtonRect = ResetButton->GetDrawRect();
		const SDL_Rect& PanelRect = m_PanelComponent->GetDrawRect();
		ResetButton->SetRectPosition(
			PanelRect.w / 2 - ButtonRect.w / 2, 
			PanelRect.h / 2 - ButtonRect.w / 2 + 100
		);
		ResetButton->SetOnClickHandler<GameOverPanel>(
			this, &GameOverPanel::ResetButtonClick_Handle);
	}
}

void GameOverPanel::ResetButtonClick_Handle(UIButtonComponent* Button)
{
	GetGame()->ResetGame();
	DestroyDeferred();
}
