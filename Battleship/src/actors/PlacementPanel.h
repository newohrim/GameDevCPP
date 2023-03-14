#pragma once

#include "ui/UIContainerActor.h"
#include "actors/Battleship.h"

class UIButtonComponent;
class TextUIComponent;

class PlacementPanel : public UIContainerActor
{
public:
	PlacementPanel(
		const std::vector<BattleshipStats>& Templates, TTF_Font* Font, Game* GameInstance);

	int GetShipsCount(BattleshipStats* Template) const;

	void AddShips(BattleshipStats* Template, int Count);

	void RemoveShips(BattleshipStats* Template, int Count) { AddShips(Template, -Count); }

	bool AnyShipsLeft() const;

private:
	struct PlacementShipItem
	{
		const BattleshipStats& m_ShipTemplate;
		UIButtonComponent* m_ShipButton;
		TextUIComponent* m_ShipTextCounter;
		int m_ShipCounter;
	};

	std::vector<PlacementShipItem> m_ShipItems;
	
	PlacementShipItem* m_SelectedItem = nullptr;

	void ShipButtonClicked_Handler(UIButtonComponent* Button);

	PlacementShipItem& GetShipItem(BattleshipStats* Template);

	PlacementShipItem& GetShipItem(UIButtonComponent* ShipButton);
};

