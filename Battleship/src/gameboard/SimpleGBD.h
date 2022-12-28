#pragma once

#include "GameBoardDrawer.h"
#include <vector>
#include "GameBoard.h"
#include "SDL/SDL.h"
#include "../ui/TextUIData.h"

class Game;

class SimpleGBD : public GameBoardDrawer
{
public:
	SimpleGBD(GameBoard* Board, Game* GameInstance, TTF_Font* Font);

	virtual void DrawBoard(
		GameBoard* Board, SDL_Renderer* Renderer, const float CellSize = 100.0f) override;

	virtual CellCoord GetWorldToBoardPos(
		GameBoard* Board, const Vector2 WorldPos, const float CellSize = 100.0f) const override;

	virtual void SetDrawVisablity(bool IsVisible, GameBoard* Board) override;

	virtual void SetShipsVisability(bool IsVisible, GameBoard* Board) override;

	virtual void SetBoardPosition(Vector2 Position, GameBoard* Board, const float CellSize = 100.0f) override;

	virtual void SetDrawUnavailableCellsHighlight(bool Value) override { m_DrawHintCellHighlight = Value; }

protected:
	virtual void DrawGrid(
		GameBoard* Board, SDL_Renderer* Renderer, const float CellSize = 100.0f);

	virtual void DrawCells(
		GameBoard* Board, SDL_Renderer* Renderer, const float CellSize = 100.0f);

	virtual void HighlightCurrentCell(
		GameBoard* Board, SDL_Renderer* Renderer, const float CellSize = 100.0f);

	virtual void DrawCellsStates(
		GameBoard* Board, SDL_Renderer* Renderer, const float CellSize = 100.0f);

	virtual void DrawMarkupBorder(
		GameBoard* Board, SDL_Renderer* Renderer, const float CellSize = 100.0f);

private:
	std::vector<SDL_Rect> Quads;

	std::vector<TextUIData> m_MarkupRow;

	std::vector<TextUIData> m_MarkupColumn;

	SDL_Texture* m_CellState_Damaged = nullptr;

	// Draws red highlight over cell when mouse over
	// If false draws yellow highlight only
	bool m_DrawHintCellHighlight = true;
};

