#include "PlacementConfirmPanel.h"

#include "ui/UIPanelComponent.h"
#include "ui/UIButtonComponent.h"
#include "core/Game.h"

PlacementConfirmPanel::PlacementConfirmPanel(
	void(Game::* Handler)(bool), void(Game::* RotateHandler)(), Game* GameInstance)
	: UIContainerActor(GameInstance)
{
	m_ConfirmPlacementHandle.m_Callback = std::bind(Handler, GameInstance, std::placeholders::_1);
	m_RotateShipHandle.m_Callback = std::bind(RotateHandler, GameInstance);

	UIPanelComponent* const Panel = new UIPanelComponent(this, -1);
	Panel->SetPanelColor({125, 125, 125 });
	Panel->SetRectDimension(136, 48);

	{
		SDL_Texture* const ConfirmButtonTex = 
			GetGame()->GetTexture("Assets/ui/confirm_button.png");
		UIButtonComponent* const ConfirmButton =
			new UIButtonComponent(ConfirmButtonTex, this, 1);
		ConfirmButton->SetRectPosition(10, 10);
		ConfirmButton->SetRectScale(0.25f);
		ConfirmButton->SetOnClickHandler<PlacementConfirmPanel>(this, &PlacementConfirmPanel::ConfirmPlacement_Handle);
	}

	{
		SDL_Texture* const RotateButtonTex =
			GetGame()->GetTexture("Assets/ui/rotate_button.png");
		UIButtonComponent* const RotateButton =
			new UIButtonComponent(RotateButtonTex, this, 1);
		RotateButton->SetRectPosition(52, 10);
		RotateButton->SetRectScale(0.25f);
		RotateButton->SetOnClickHandler<PlacementConfirmPanel>(this, &PlacementConfirmPanel::RotateShip_Handle);
	}

	{
		SDL_Texture* const RotateButtonTex =
			GetGame()->GetTexture("Assets/ui/cancel_button.png");
		UIButtonComponent* const RotateButton =
			new UIButtonComponent(RotateButtonTex, this, 1);
		RotateButton->SetRectPosition(92, 10);
		RotateButton->SetRectScale(0.25f);
		RotateButton->SetOnClickHandler<PlacementConfirmPanel>(this, &PlacementConfirmPanel::CancelPlacement_Handle);
	}
}

void PlacementConfirmPanel::ConfirmPlacement_Handle(UIButtonComponent* Button)
{
	if (m_ConfirmPlacementHandle.m_Callback)
	{
		m_ConfirmPlacementHandle.m_Callback(true);
	}
}

void PlacementConfirmPanel::CancelPlacement_Handle(UIButtonComponent* Button)
{
	if (m_ConfirmPlacementHandle.m_Callback)
	{
		m_ConfirmPlacementHandle.m_Callback(false);
	}
}

void PlacementConfirmPanel::RotateShip_Handle(UIButtonComponent* Button)
{
	if (m_RotateShipHandle.m_Callback) 
	{
		m_RotateShipHandle.m_Callback();
	}
}
