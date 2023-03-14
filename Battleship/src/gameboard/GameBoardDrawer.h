#pragma once

#include "core/Math.h"
#include "gameboard/GameBoard.h"

class GameBoard;
class BoardCell;
class SDL_Renderer;

class GameBoardDrawer
{
public:
	GameBoardDrawer(float CellSize);

	virtual void DrawBoard(GameBoard* Board, SDL_Renderer* Renderer) = 0;

	virtual CellCoord GetWorldToBoardPos(const Vector2 WorldPos) const = 0;

	BoardCell* GetCurrMouseOverCell() const { return m_CurrMouseOverCell; }

	void SetCurrMouseOverCell(BoardCell* CurrMouseOverCell) { m_CurrMouseOverCell = CurrMouseOverCell; }

	void ClearCurrMouseOverCell() { m_CurrMouseOverCell = nullptr; }

	bool IsVisible() const { return m_IsVisible; }

	virtual void SetDrawVisablity(GameBoard* Board, bool IsVisible) { m_IsVisible = IsVisible; }

	virtual void SetShipsVisability(GameBoard* Board, bool IsVisible) = 0;

	Vector2 GetBoardPosition() const { return m_BoardPosition; }

	virtual void SetBoardPosition(GameBoard* Board, Vector2 Position) { m_BoardPosition = Position; }

	virtual void SetDrawUnavailableCellsHighlight(bool Value) = 0;

	float GetCellSize() const { return m_CellSize; }

	void SetCellSize(float CellSize) { m_CellSize = CellSize; }

protected:
	BoardCell* m_CurrMouseOverCell = nullptr;

	Vector2 m_BoardPosition;

	bool m_IsVisible = true;

	float m_CellSize;
};

