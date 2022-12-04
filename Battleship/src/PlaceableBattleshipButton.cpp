#include "PlaceableBattleshipButton.h"

#include "SpriteComponent.h"
#include "RectangleClickZone.h"
#include "TextUIComponent.h"
#include "Battleship.h"

#include <algorithm>
#include <string>

PlaceableBattleshipButton::PlaceableBattleshipButton(
	const BattleshipStats& ShipStats, const int BeginCount, TTF_Font* TextFont, Game* GameInstance) 
	: Actor(GameInstance), m_ShipStats(&ShipStats), m_ShipsLeft(BeginCount)
{
	m_SpriteComponent = new SpriteComponent(this);
	m_SpriteComponent->SetTexture(m_ShipStats->m_ShipTexture);
	int TexWidth, TexHeight;
	SDL_QueryTexture(m_ShipStats->m_ShipTexture, nullptr, nullptr, &TexWidth, &TexHeight);
	// Ship buttons are rotated 90 degrees, so need to swap values
	std::swap(TexWidth, TexHeight);
	m_ClickZone = new RectangleClickZone(TexWidth, TexHeight, this);
	m_TextComponent = new TextUIComponent(TextFont, this);
	const std::string Text("x" + std::to_string(m_ShipsLeft));
	m_TextComponent->SetText(Text);
	m_TextComponent->SetTextScale(1.0f);
	m_TextComponent->SetTextColor({ 255, 255, 255 });
}

bool PlaceableBattleshipButton::DecrementShipsCount()
{
	const bool Result = m_ShipsLeft > 0;
	m_ShipsLeft = std::max(m_ShipsLeft - 1, 0);
	const std::string Text("x" + std::to_string(m_ShipsLeft));
	m_TextComponent->SetText(Text);
	return Result;
}
