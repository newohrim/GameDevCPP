#pragma once

#include "gameboard/GameBoardDrawer.h"
#include <vector>
#include "gameboard/GameBoard.h"
#include "SDL/SDL.h"
#include "ui/TextUIData.h"

class Game;

class SimpleGBD : public GameBoardDrawer
{
public:
	SimpleGBD(int Width, int Height, float CellSize, Game* GameInstance, TTF_Font* Font);

	virtual void DrawBoard(GameBoard* Board, SDL_Renderer* Renderer) override;

	virtual CellCoord GetWorldToBoardPos(const Vector2 WorldPos) const override;

	virtual void SetDrawVisablity(GameBoard* Board, bool IsVisible) override;

	virtual void SetShipsVisability(GameBoard* Board, bool IsVisible) override;

	virtual void SetBoardPosition(GameBoard* Board, Vector2 Position) override;

	virtual void SetDrawUnavailableCellsHighlight(bool Value) override { m_DrawHintCellHighlight = Value; }

protected:
	virtual void DrawGrid(SDL_Renderer* Renderer);

	virtual void DrawCells(SDL_Renderer* Renderer);

	virtual void HighlightCurrentCell(SDL_Renderer* Renderer);

	virtual void DrawCellsStates(GameBoard* Board, SDL_Renderer* Renderer);

	virtual void DrawMarkupBorder(SDL_Renderer* Renderer);

private:
	std::vector<SDL_Rect> Quads;

	std::vector<TextUIData> m_MarkupRow;

	std::vector<TextUIData> m_MarkupColumn;

	SDL_Texture* m_CellState_Damaged = nullptr;

	// Draws red highlight over cell when mouse over
	// If false draws yellow highlight only
	bool m_DrawHintCellHighlight = true;
};

