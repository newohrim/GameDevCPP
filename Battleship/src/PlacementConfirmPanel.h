#pragma once

#include "ui/UIContainerActor.h"
#include <functional>

class UIButtonComponent;
using ConfirmationHandler = void (*)(bool);

namespace ConfirmPanel 
{
	struct Callback 
	{
		std::function<void(bool)> m_Callback;
	};
}

class PlacementConfirmPanel : public UIContainerActor
{
public:
	PlacementConfirmPanel(void(Game::*Handler)(bool), Game* GameInstance);

private:
	ConfirmPanel::Callback m_ConfirmPlacementHandle;

	void ConfirmPlacement_Handle(UIButtonComponent* Button);
};
