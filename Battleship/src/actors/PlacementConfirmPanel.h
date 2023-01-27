#pragma once

#include "ui/UIContainerActor.h"
#include <functional>

class UIButtonComponent;
using ConfirmationHandler = void (*)(bool);

namespace ConfirmPanel 
{
	struct CallbackBool
	{
		std::function<void(bool)> m_Callback;
	};

	struct Callback 
	{
		std::function<void()> m_Callback;
	};
}

class PlacementConfirmPanel : public UIContainerActor
{
public:
	PlacementConfirmPanel(
		void(SBGame::*PlacementHandler)(bool), void(SBGame::*RotateHandler)(), Game* GameInstance);

private:
	ConfirmPanel::CallbackBool m_ConfirmPlacementHandle;

	ConfirmPanel::Callback m_RotateShipHandle;

	void ConfirmPlacement_Handle(UIButtonComponent* Button);

	void CancelPlacement_Handle(UIButtonComponent* Button);

	void RotateShip_Handle(UIButtonComponent* Button);
};
