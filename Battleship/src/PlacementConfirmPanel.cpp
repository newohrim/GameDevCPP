#include "PlacementConfirmPanel.h"

#include "ui/UIPanelComponent.h"
#include "ui/UIButtonComponent.h"
#include "Game.h"

PlacementConfirmPanel::PlacementConfirmPanel(void(Game::* Handler)(bool), Game* GameInstance)
	: UIContainerActor(GameInstance)
{
	m_ConfirmPlacementHandle.m_Callback = std::bind(Handler, GameInstance, true);

	UIPanelComponent* const Panel = new UIPanelComponent(this, -1);
	Panel->SetPanelColor({125, 125, 125 });
	Panel->SetRectDimension(100, 48);

	SDL_Texture* const ConfirmButtonTex = 
		GetGame()->GetTexture("Assets/ui/confirm_button.png");
	UIButtonComponent* const Button = 
		new UIButtonComponent(ConfirmButtonTex, this, 1);
	Button->SetRectPosition(10, 10);
	Button->SetRectScale(0.25f);
	Button->SetOnClickHandler<PlacementConfirmPanel>(this, &PlacementConfirmPanel::ConfirmPlacement_Handle);
}

void PlacementConfirmPanel::ConfirmPlacement_Handle(UIButtonComponent* Button)
{
	if (m_ConfirmPlacementHandle.m_Callback)
	{
		m_ConfirmPlacementHandle.m_Callback(true);
	}
}
