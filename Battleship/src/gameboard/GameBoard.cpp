#include "GameBoard.h"

#include <algorithm>
#include "SimpleGBP.h"
#include "SimpleGBD.h"

GameBoard::GameBoard(const uint8_t BoardWidth, const uint8_t BoardHeight)
{
	m_Grid = BoardGrid(BoardHeight, GridRow(BoardWidth));
	for (uint8_t y = 0; y < BoardHeight; ++y) 
	{
		for (uint8_t x = 0; x < BoardWidth; ++x) 
		{
			m_Grid[y][x] = BoardCell({ x, y });
		}
	}

	BoardPopulator = new SimpleGBP();
	BoardDrawer = new SimpleGBD(this);
}

GameBoard::~GameBoard()
{
	delete BoardPopulator;
}

bool GameBoard::AddShip(Battleship* Ship, const CellCoord Pos)
{
	if (!IsValidCoords(Pos) || !IsAvailableForShip(Pos, Ship))
	{
		return false;
	}

	InsertShip(Ship, Pos);
	return true;
}

bool GameBoard::IsAvailableForShip(
	const CellCoord CellCoords, 
	BattleshipStats const* ShipStats, 
	const ShipOritentation ShipOrient)
{
	const CellCoord Direction = 
		ShipOrient == ShipOritentation::Horizontal 
		? CellCoord{ 1, 0 } : CellCoord{ 0, 1 };
	if (!IsValidCoords(CellCoords) || 
		!IsValidCoords(CellCoords + Direction * (ShipStats->m_ShipLength - 1))) 
	{
		return false;
	}

	for (uint8_t Offset = 0; Offset < ShipStats->m_ShipLength; ++Offset) 
	{
		const CellCoord Coord = CellCoords + Direction * Offset;
		const BoardCell& Cell = GetCell(Coord);
		if (!Cell.IsEmpty() || !Cell.IsAvailableForShip())
		{
			return false;
		}
	}
	
	return true;
}

void GameBoard::InsertShip(Battleship* Ship, const CellCoord Pos)
{
	const uint8_t ShipLength = Ship->GetShipStats()->m_ShipLength;
	const CellCoord Direction = 
		Ship->GetShipOrientation() == ShipOritentation::Horizontal 
		? CellCoord{ 1, 0 } : CellCoord{ 0, 1 };
	for (uint8_t Offset = 0; Offset < ShipLength; ++Offset)
	{
		const CellCoord Coord = Pos + Direction * Offset;
		BoardCell& Cell = GetCell_Private(Coord);
		Cell.SetShip(Ship);
		MakePaddingAround(Coord);
	}
}

void GameBoard::MakePaddingAround(const CellCoord Coords)
{
	std::vector<CellCoord> PaddingAround = GetNeighbourCoords(Coords);
	for (CellCoord C : PaddingAround) 
	{
		BoardCell& Cell = GetCell_Private(C);
		Cell.SetAvaiablityForShip(false);
	}
}

std::vector<CellCoord> GameBoard::GetNeighbourCoords(const CellCoord Coords) const
{
	std::vector<CellCoord> Result;
	for (uint8_t x = std::max(Coords.x, (uint8_t)1U) - 1; x < std::min((uint8_t)(Coords.x + 2U), GetBoardWidth()); ++x) {
		for (uint8_t y = std::max(Coords.y, (uint8_t)1U) - 1; y < std::min((uint8_t)(Coords.y + 2U), GetBoardHeight()); ++y) {
			if (x == Coords.x && y == Coords.y) continue;
			Result.push_back({ x, y });
		}
	}

	if (Result.size() > 8) 
	{
		return Result;
	}

	return Result;
}
