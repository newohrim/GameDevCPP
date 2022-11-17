#include "SimpleGBD.h"

#include "../SpriteComponent.h"

#ifndef CACHE_COLOR_IN
#define CACHE_COLOR_IN() \
SDL_Color CachedColor; \
SDL_GetRenderDrawColor( \
	Renderer, &CachedColor.r, &CachedColor.g, &CachedColor.b, &CachedColor.a);
#endif

#ifndef CACHE_COLOR_OUT
#define CACHE_COLOR_OUT() \
SDL_SetRenderDrawColor( \
	Renderer, CachedColor.r, CachedColor.g, CachedColor.b, CachedColor.a);
#endif

SimpleGBD::SimpleGBD(GameBoard* Board)
{
	for (uint8_t y = 0; y < Board->GetBoardHeight(); ++y) 
	{
		for (uint8_t x = 0; x < Board->GetBoardWidth(); ++x) 
		{
			Quads.push_back(SDL_Rect{ x, y, 1, 1 });
		}
	}
}

void SimpleGBD::DrawBoard(GameBoard* Board, SDL_Renderer* Renderer, const float CellSize)
{
	if (!IsVisible())
		return;

	DrawGrid(Board, Renderer, CellSize);
	HighlightCurrentCell(Board, Renderer, CellSize);
}

CellCoord SimpleGBD::GetWorldToBoardPos(GameBoard* Board, const Vector2 WorldPos, const float CellSize) const
{
	const CellCoord Result = 
	{ 
		(uint8_t)(WorldPos.x / CellSize), 
		(uint8_t)(WorldPos.y / CellSize) 
	};

	return Result;
}

void SimpleGBD::SetDrawVisablity(bool IsVisible, GameBoard* Board)
{
	GameBoardDrawer::SetDrawVisablity(IsVisible, Board);

	const std::vector<Battleship*>& Ships = Board->GetShipsOnBoard();
	for (Battleship* Ship : Ships) 
	{
		Ship->GetSpriteComponent()->SetIsVisible(IsVisible);
	}
}

void SimpleGBD::DrawGrid(GameBoard* Board, SDL_Renderer* Renderer, const float CellSize)
{
	CACHE_COLOR_IN();

	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
	for (SDL_Rect Rect : Quads)
	{
		Rect.x *= CellSize;
		Rect.y *= CellSize;
		Rect.w *= CellSize;
		Rect.h *= CellSize;
		SDL_RenderDrawRect(Renderer, &Rect);
	}
	//SDL_RenderDrawRects(Renderer, Quads.data, Quads.size());

	CACHE_COLOR_OUT();
}

void SimpleGBD::DrawCells(GameBoard* Board, SDL_Renderer* Renderer, const float CellSize)
{
	CACHE_COLOR_IN();

	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
	for (SDL_Rect Rect : Quads)
	{
		Rect.x *= CellSize;
		Rect.y *= CellSize;
		Rect.w *= CellSize;
		Rect.h *= CellSize;
		SDL_RenderFillRect(Renderer, &Rect);
	}
	//SDL_RenderDrawRects(Renderer, Quads.data, Quads.size());

	CACHE_COLOR_OUT();
}

void SimpleGBD::HighlightCurrentCell(GameBoard* Board, SDL_Renderer* Renderer, const float CellSize)
{
	CACHE_COLOR_IN();

	const BoardCell* CurrMouseOver = GetCurrMouseOverCell();
	if (CurrMouseOver) 
	{
		const SDL_Color DrawColor = 
			CurrMouseOver->IsAvailableForShip() ? 
			SDL_Color{ 255, 255, 0, 255 } : 
			SDL_Color{ 255, 0, 0, 255 };
		SDL_SetRenderDrawColor(Renderer, DrawColor.r, DrawColor.g, DrawColor.b, DrawColor.a);

		const CellCoord Coord = CurrMouseOver->GetCellCoords();
		const SDL_Rect HightlightRect = 
		{ 
			Coord.x * CellSize, 
			Coord.y * CellSize, 
			CellSize, 
			CellSize 
		};
		SDL_RenderDrawRect(Renderer, &HightlightRect);
	}

	CACHE_COLOR_OUT();
}
