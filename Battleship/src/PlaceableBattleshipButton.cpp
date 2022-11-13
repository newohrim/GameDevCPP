#include "PlaceableBattleshipButton.h"

#include "SpriteComponent.h"
#include "RectangleClickZone.h"
#include "Battleship.h"

#include <algorithm>

PlaceableBattleshipButton::PlaceableBattleshipButton(
	const BattleshipStats& ShipStats, const int BeginCount, Game* GameInstance) 
	: Actor(GameInstance), m_ShipStats(&ShipStats), m_ShipsLeft(BeginCount)
{
	m_SpriteComponent = new SpriteComponent(this);
	m_SpriteComponent->SetTexture(m_ShipStats->m_ShipTexture);
	int TexWidth, TexHeight;
	SDL_QueryTexture(m_ShipStats->m_ShipTexture, nullptr, nullptr, &TexWidth, &TexHeight);
	// Ship buttons are rotated 90 degrees, so need to swap values
	std::swap(TexWidth, TexHeight);
	m_ClickZone = new RectangleClickZone(TexWidth, TexHeight, this);
}

bool PlaceableBattleshipButton::DecrementShipsCount()
{
	const bool Result = m_ShipsLeft > 0;
	m_ShipsLeft = std::max(m_ShipsLeft - 1, 0);
	return Result;
}
