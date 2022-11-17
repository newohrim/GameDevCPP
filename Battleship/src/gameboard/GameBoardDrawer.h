#pragma once

#include "../Math.h"
#include "GameBoard.h"

class GameBoard;
class BoardCell;
class SDL_Renderer;

class GameBoardDrawer
{
public:
	virtual void DrawBoard(GameBoard* Board, SDL_Renderer* Renderer, const float CellSize = 100.0f) = 0;

	virtual CellCoord GetWorldToBoardPos(GameBoard* Board, const Vector2 WorldPos, const float CellSize = 100.0f) const = 0;

	BoardCell* GetCurrMouseOverCell() const { return m_CurrMouseOverCell; }

	void SetCurrMouseOverCell(BoardCell* CurrMouseOverCell) { m_CurrMouseOverCell = CurrMouseOverCell; }

	void ClearCurrMouseOverCell() { m_CurrMouseOverCell = nullptr; }

	bool IsVisible() const { return m_IsVisible; }

	virtual void SetDrawVisablity(bool IsVisible, GameBoard* Board) { m_IsVisible = IsVisible; }

private:
	BoardCell* m_CurrMouseOverCell = nullptr;

	bool m_IsVisible = true;
};

