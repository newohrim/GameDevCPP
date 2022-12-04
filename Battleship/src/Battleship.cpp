#include "Battleship.h"

//#include "SDL/SDL.h"
#include "SpriteComponent.h"

Battleship::Battleship(
	BattleshipStats const* ShipStats,
	const ShipOrientation ShipOrient, 
	const CellCoord CoordOnBoard, 
	Game* GameInstance)
	: Actor(GameInstance), 
	m_ShipStats(ShipStats), 
	m_ShipOrientation(ShipOrient), 
	m_ShipOnBoardCoords(CoordOnBoard),
	m_ShipHealth(ShipStats->m_ShipLength)
{
	m_SpriteComponent = new SpriteComponent(this);
	m_SpriteComponent->SetTexture(m_ShipStats->m_ShipTexture);
}

void Battleship::TakeDamageHandle()
{
	if (m_ShipHealth > 0)
		m_ShipHealth--;
}
