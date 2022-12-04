#pragma once

#include "Actor.h"

#include "SDL_ttf.h"

class SpriteComponent;
class RectangleClickZone;
class TextUIComponent;
struct BattleshipStats;

class PlaceableBattleshipButton : public Actor
{
public:
	PlaceableBattleshipButton(const BattleshipStats& ShipStats, const int BeginCount, TTF_Font* TextFont, Game* GameInstance);

	bool IsEmpty() const { return m_ShipsLeft == 0; }

	bool DecrementShipsCount();

	SpriteComponent* GetSpriteComponent() const { return m_SpriteComponent; }

	RectangleClickZone* GetRectangleClickZone() const { return m_ClickZone; }

	TextUIComponent* GetTextComponent() const { return m_TextComponent; }

	const BattleshipStats* GetShipStats() const { return m_ShipStats; }

protected:
	SpriteComponent* m_SpriteComponent;

	RectangleClickZone* m_ClickZone;

	TextUIComponent* m_TextComponent;

	const BattleshipStats* m_ShipStats;

	int m_ShipsLeft;
};

