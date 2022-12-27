#pragma once

#include <vector>
//#include "../Battleship.h"
#include "../Math.h"
#include "SDL_ttf.h"

class BoardCell;
using GridRow = std::vector<BoardCell>;
using BoardGrid = std::vector<GridRow>;

class GameBoardPopulator;
class GameBoardDrawer;
class Battleship;
struct BattleshipStats;
class Game;
enum ShipOrientation;

// TODO: Make immutable
struct CellCoord 
{
	uint8_t x = 0;
	uint8_t y = 0;
};

enum CellState 
{
	Empty = 0,
	ShipPlaced,
	ShipDamaged,
	Missed
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

static bool operator==(const CellCoord& A, const CellCoord& B)
{
	return (A.x == B.x) && (A.y == B.y);
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
		m_CellState = CellState::ShipPlaced;
		m_IsAvailableForShip = false;
	}

	void ClearShip() 
	{ 
		m_ContainedShip = nullptr; 
		m_CellState = CellState::Empty;
		// TODO: Clear availability for neighboors
	}

	bool IsEmpty() const { return m_CellState == CellState::Empty; }

	bool IsAvailableForShip() const { return m_IsAvailableForShip; }

	void SetAvaiablityForShip(bool Value) { m_IsAvailableForShip = Value; }

	CellState GetCellState() const { return m_CellState; }

	void SetCellState(CellState State);

private:
	CellCoord m_Coords;

	Battleship* m_ContainedShip = nullptr;

	CellState m_CellState = CellState::Empty;

	bool m_IsAvailableForShip = true;
};

class GameBoard
{
public:
	GameBoard(const uint8_t BoardWidth, const uint8_t BoardHeight, Game* GameInstance, TTF_Font* Font);

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
		ShipOrientation const* ShipOrient);

	// TODO: Make const
	bool IsAvailableForShip(
		const CellCoord CellCoords,
		Battleship* Ship);

	Vector2 GetCorrectShipPosition(Battleship* Ship, const float CellSize = 100.0f) const;

	float GetCorrectShipRotation(Battleship* Ship) const;

	GameBoardPopulator* GetBoardPopulator() const { return BoardPopulator; }

	GameBoardDrawer* GetBoardDrawer() const { return BoardDrawer; }

	const std::vector<Battleship*>& GetShipsOnBoard() const { return m_Ships; }

	void SetShipOrientation(Battleship* Ship, ShipOrientation Orientation);

	bool CheckGameOverCondition() const;

	std::vector<CellCoord> GetVonNeumannNeighborhood(const CellCoord Coords, int Margin = 0) const;

	std::vector<CellCoord> GetMooreNeighborhood(const CellCoord Coords) const;

	std::vector<CellCoord> GetCornersNeighborhood(const CellCoord Coords, int Margin = 0) const;

protected:
	GameBoardPopulator* BoardPopulator;

	GameBoardDrawer* BoardDrawer;

private:
	BoardGrid m_Grid;

	std::vector<Battleship*> m_Ships;

	BoardCell& GetCell_Private(const CellCoord Coords) { return m_Grid[Coords.y][Coords.x]; }

	void InsertShip(Battleship* Ship, const CellCoord Pos);

	void MakePaddingAround(const CellCoord Coords);
};

