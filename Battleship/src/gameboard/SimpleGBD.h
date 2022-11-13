#pragma once

#include "GameBoardDrawer.h"
#include <vector>
#include "GameBoard.h"
#include "SDL/SDL.h"

class SimpleGBD : public GameBoardDrawer
{
public:
	SimpleGBD(GameBoard* Board);

	virtual void DrawBoard(
		GameBoard* Board, SDL_Renderer* Renderer, const float CellSize = 100.0f) override;

	virtual CellCoord GetWorldToBoardPos(
		GameBoard* Board, const Vector2 WorldPos, const float CellSize = 100.0f) const override;

protected:
	virtual void DrawGrid(
		GameBoard* Board, SDL_Renderer* Renderer, const float CellSize = 100.0f);

	virtual void DrawCells(
		GameBoard* Board, SDL_Renderer* Renderer, const float CellSize = 100.0f);

	virtual void HighlightCurrentCell(
		GameBoard* Board, SDL_Renderer* Renderer, const float CellSize = 100.0f);

private:
	std::vector<SDL_Rect> Quads;
};

