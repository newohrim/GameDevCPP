#pragma once

#include "Actor.h"
#include "gameboard/GameBoard.h"

struct SDL_Texture;
class SpriteComponent;

enum ShipOrientation
{
	Horizontal = 0,
	Vertical = 1
};

struct BattleshipStats 
{
	SDL_Texture* m_ShipTexture = nullptr;
	uint8_t m_ShipLength = 1;
	uint8_t m_ShipsBeginCount = 1;
};

class Battleship : public Actor
{
public:
	Battleship(
		BattleshipStats const* ShipStats, 
		const ShipOrientation ShipOrient, 
		const CellCoord CoordOnBoard, 
		Game* GameInstance);

	BattleshipStats const* GetShipStats() const { return m_ShipStats; }

	CellCoord GetShipOnBoardCoords() const { return m_ShipOnBoardCoords; }

	ShipOrientation GetShipOrientation() const { return m_ShipOrientation; }

	SpriteComponent* GetSpriteComponent() const { return m_SpriteComponent; }

protected:
	BattleshipStats const* const m_ShipStats;

	const ShipOrientation m_ShipOrientation;

	SpriteComponent* m_SpriteComponent;

private:
	CellCoord m_ShipOnBoardCoords;
};

