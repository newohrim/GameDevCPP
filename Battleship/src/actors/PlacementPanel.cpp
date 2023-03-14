#include "PlacementPanel.h"

#include "ui/UIButtonComponent.h"
#include "components/TextUIComponent.h"
#include "SBGame.h"
#include <algorithm>

PlacementPanel::PlacementPanel(
	const std::vector<BattleshipStats>& Templates, TTF_Font* Font, Game* GameInstance)
	: UIContainerActor(GameInstance)
{
	for (const BattleshipStats& Ship : Templates) 
	{
		m_ShipItems.push_back({ Ship, nullptr, nullptr, Ship.m_ShipsBeginCount });
	}

	int OffsetY = 0;
	constexpr float ButtonAngle = Math::Pi / 2.0f;

	for (int i = 0; i < Templates.size(); ++i) 
	{
		{
			UIButtonComponent* const ShipButton = 
				new UIButtonComponent(Templates[i].m_ShipTexture, this, 1);
			OffsetY += 50;
			ShipButton->SetRectPosition(0, OffsetY);
			ShipButton->SetRectAngle(ButtonAngle);
			ShipButton->SetPivotInMiddle();
			ShipButton->SetOnClickHandler<PlacementPanel>(
				this, &PlacementPanel::ShipButtonClicked_Handler);
			m_ShipItems[i].m_ShipButton = ShipButton;
		}
		{
			TextUIComponent* const TextCounter = 
				new TextUIComponent(Font, this, 1);
			const std::string Text("x" + std::to_string(
				Templates[i].m_ShipsBeginCount));
			TextCounter->SetText(Text);
			TextCounter->SetRectScale(1.0f);
			TextCounter->SetTextColor({ 255, 255, 255 });
			TextCounter->SetRectPosition(120, OffsetY - 20);
			m_ShipItems[i].m_ShipTextCounter = TextCounter;
		}
	}
}

int PlacementPanel::GetShipsCount(BattleshipStats* Template) const
{
	PlacementShipItem& ShipItem = 
		const_cast<PlacementPanel*>(this)->GetShipItem(Template);
	return ShipItem.m_ShipCounter;
}

void PlacementPanel::AddShips(BattleshipStats* Template, int Count)
{
	PlacementPanel::PlacementShipItem& ShipItem = GetShipItem(Template);
	ShipItem.m_ShipCounter = std::max(0, ShipItem.m_ShipCounter + Count);
	const std::string Text("x" + std::to_string(ShipItem.m_ShipCounter));
	ShipItem.m_ShipTextCounter->SetText(Text);
}

bool PlacementPanel::AnyShipsLeft() const
{
	for (const PlacementShipItem& ShipItem : m_ShipItems) 
	{
		if (ShipItem.m_ShipCounter > 0) 
		{
			return true;
		}
	}

	return false;
}

void PlacementPanel::ShipButtonClicked_Handler(UIButtonComponent* Button)
{
	PlacementPanel::PlacementShipItem& ShipItem = GetShipItem(Button);
	if (ShipItem.m_ShipCounter > 0) 
	{
		if (m_SelectedItem)
		{
			m_SelectedItem->m_ShipButton->
				SetColorMultiplier({ 255, 255, 255, 255 });
		}
		m_SelectedItem = &ShipItem;
		ShipItem.m_ShipButton->
			SetColorMultiplier({255, 255, 0, 255});

		static_cast<SBGame*>(GetGame())->
			ShipTemplateSelected_Handle(
				const_cast<BattleshipStats&>(
					ShipItem.m_ShipTemplate));
	}
}

PlacementPanel::PlacementShipItem& PlacementPanel::GetShipItem(BattleshipStats* Template)
{
	for (PlacementShipItem& Item : m_ShipItems) 
	{
		if (&Item.m_ShipTemplate == Template) 
		{
			return Item;
		}
	}

	// TODO: Throw exception here?
	return m_ShipItems[0];
}

PlacementPanel::PlacementShipItem& PlacementPanel::GetShipItem(UIButtonComponent* ShipButton)
{
	for (PlacementShipItem& Item : m_ShipItems)
	{
		if (Item.m_ShipButton == ShipButton)
		{
			return Item;
		}
	}

	// TODO: Throw exception here?
	return m_ShipItems[0];
}
