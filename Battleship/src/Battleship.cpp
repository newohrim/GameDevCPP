#include "Battleship.h"

//#include "SDL/SDL.h"
#include "SpriteComponent.h"

Battleship::Battleship(BattleshipStats const* ShipStats, 
	const ShipOritentation ShipOrient, Game* GameInstance)
	: Actor(GameInstance), m_ShipStats(ShipStats), m_ShipOrientation(ShipOrient)
{
	m_SpriteComponent = new SpriteComponent(this);
	m_SpriteComponent->SetTexture(m_ShipStats->m_ShipTexture);
}
