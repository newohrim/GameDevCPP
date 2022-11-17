#pragma once

#include "Actor.h"

class SpriteComponent;
class RectangleClickZone;
struct BattleshipStats;

class PlaceableBattleshipButton : public Actor
{
public:
	PlaceableBattleshipButton(const BattleshipStats& ShipStats, const int BeginCount, Game* GameInstance);

	bool IsEmpty() const { return m_ShipsLeft == 0; }

	bool DecrementShipsCount();

	SpriteComponent* GetSpriteComponent() const { return m_SpriteComponent; }

	RectangleClickZone* GetRectangleClickZone() const { return m_ClickZone; }

	const BattleshipStats* GetShipStats() const { return m_ShipStats; }

protected:
	SpriteComponent* m_SpriteComponent;

	RectangleClickZone* m_ClickZone;

	const BattleshipStats* m_ShipStats;

	int m_ShipsLeft;
};

