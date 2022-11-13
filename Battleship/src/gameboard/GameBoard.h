#pragma once

#include <vector>
#include "../Battleship.h"

class BoardCell;
using GridRow = std::vector<BoardCell>;
using BoardGrid = std::vector<GridRow>;

class GameBoardPopulator;
class GameBoardDrawer;

// TODO: Make immutable
struct CellCoord 
{
	uint8_t x = 0;
	uint8_t y = 0;
};

static CellCoord operator+(const CellCoord& A, const CellCoord& B)
{
	return CellCoord{ (uint8_t)(A.x + B.x), (uint8_t)(A.y + B.y) };
}

static CellCoord operator-(const CellCoord& A, const CellCoord& B)
{
	return CellCoord{ (uint8_t)(A.x - B.x), (uint8_t)(A.y - B.y) };
}

static CellCoord operator*(const CellCoord& A, const int B)
{
	return CellCoord{ static_cast<uint8_t>(A.x * B), static_cast<uint8_t>(A.y * B) };
}

class BoardCell 
{
public:
	BoardCell() = default;

	BoardCell(CellCoord Coords) { m_Coords = Coords; }

	CellCoord GetCellCoords() const { return m_Coords; }

	Battleship* GetShip() const { return m_ContainedShip; }

	void SetShip(Battleship* Ship) 
	{ 
		m_ContainedShip = Ship; 
		m_IsAvailableForShip = false;
	}

	void ClearShip() { m_ContainedShip = nullptr; }

	bool IsEmpty() const { return m_ContainedShip == nullptr; }

	bool IsAvailableForShip() const { return m_IsAvailableForShip; }

	void SetAvaiablityForShip(bool Value) { m_IsAvailableForShip = Value; }

private:
	CellCoord m_Coords;

	Battleship* m_ContainedShip = nullptr;

	bool m_IsAvailableForShip = true;
};

class GameBoard
{
public:
	GameBoard(const uint8_t BoardWidth, const uint8_t BoardHeight);

	~GameBoard();

	uint8_t GetBoardWidth() const 
	{ 
		if (!m_Grid.empty()) 
			return m_Grid[0].size(); 
		return 0;
	}

	uint8_t GetBoardHeight() const { return m_Grid.size(); }
	
	bool IsValidCoords(const CellCoord Coords) const 
	{ 
		return Coords.x < GetBoardWidth() && Coords.y < GetBoardHeight(); 
	}

	// TODO: Make const
	BoardCell& GetCell(const CellCoord Coords) { return m_Grid[Coords.y][Coords.x]; }

	bool AddShip(Battleship* Ship, const CellCoord Pos);

	// TODO: Make const
	bool IsAvailableForShip(
		const CellCoord CellCoords,
		BattleshipStats const* ShipStats,
		const ShipOritentation ShipOrient);

	// TODO: Make const
	bool IsAvailableForShip(
		const CellCoord CellCoords,
		Battleship* Ship) 
	{
		return IsAvailableForShip(
			CellCoords, Ship->GetShipStats(), Ship->GetShipOrientation());
	}

	GameBoardPopulator* GetBoardPopulator() const { return BoardPopulator; }

	GameBoardDrawer* GetBoardDrawer() const { return BoardDrawer; }

protected:
	GameBoardPopulator* BoardPopulator;

	GameBoardDrawer* BoardDrawer;

private:
	BoardGrid m_Grid;

	BoardCell& GetCell_Private(const CellCoord Coords) { return m_Grid[Coords.y][Coords.x]; }

	void InsertShip(Battleship* Ship, const CellCoord Pos);

	void MakePaddingAround(const CellCoord Coords);

	std::vector<CellCoord> GetNeighbourCoords(const CellCoord Coords) const;
};

