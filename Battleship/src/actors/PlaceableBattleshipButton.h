#pragma once

#include "ui/UIButtonComponent.h"
#include "SDL_ttf.h"

class Game;
class SpriteComponent;
class TextUIComponent;
struct BattleshipStats;

class PlaceableBattleshipButton : public UIButtonComponent
{
public:
	PlaceableBattleshipButton(
		const BattleshipStats& ShipStats, const int BeginCount, TTF_Font* TextFont, UIContainerActor* Container, SDL_Texture* ButtonTexture, Game* GameInstance);

	bool IsEmpty() const { return m_ShipsLeft == 0; }

	bool DecrementShipsCount();

	TextUIComponent* GetTextComponent() const { return m_TextComponent; }

	const BattleshipStats* GetShipStats() const { return m_ShipStats; }

protected:
	TextUIComponent* m_TextComponent;

	const BattleshipStats* m_ShipStats;

	int m_ShipsLeft;
};

