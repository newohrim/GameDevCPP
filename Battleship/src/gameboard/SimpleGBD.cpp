#include "SimpleGBD.h"

#include "../SpriteComponent.h"
#include "../Game.h"

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

SimpleGBD::SimpleGBD(GameBoard* Board, Game* GameInstance, TTF_Font* Font)
	: m_MarkupRow(Board->GetBoardWidth()), 
	  m_MarkupColumn(Board->GetBoardHeight())
{
	for (uint8_t y = 0; y < Board->GetBoardHeight(); ++y) 
	{
		for (uint8_t x = 0; x < Board->GetBoardWidth(); ++x) 
		{
			Quads.push_back(SDL_Rect{ x, y, 1, 1 });
		}
	}

	char MarkupSybmol = 'A';
	for (uint8_t i = 0; i < Board->GetBoardWidth(); ++i)
	{
		m_MarkupRow[i] = TextUIData(Font, std::string(1, MarkupSybmol), GameInstance->GetRenderer());
		if (MarkupSybmol < 127)
			MarkupSybmol++;
	}

	for (uint8_t i = 0; i < Board->GetBoardHeight(); ++i)
	{
		m_MarkupColumn[i] = TextUIData(Font, std::to_string(i + 1), GameInstance->GetRenderer());
	}

	m_CellState_Damaged = 
		GameInstance->GetTexture("Assets/cells/cell_redcross.png");
}

void SimpleGBD::DrawBoard(GameBoard* Board, SDL_Renderer* Renderer, const float CellSize)
{
	if (!IsVisible())
		return;

	DrawMarkupBorder(Board, Renderer, CellSize);
	DrawGrid(Board, Renderer, CellSize);
	DrawCellsStates(Board, Renderer, CellSize);
	HighlightCurrentCell(Board, Renderer, CellSize);
}

CellCoord SimpleGBD::GetWorldToBoardPos(GameBoard* Board, const Vector2 WorldPos, const float CellSize) const
{
	const Vector2 LocalPos = WorldPos - GetBoardPosition();
	const CellCoord Result = 
	{ 
		(uint8_t)(LocalPos.x / CellSize),
		(uint8_t)(LocalPos.y / CellSize)
	};

	return Result;
}

void SimpleGBD::SetDrawVisablity(bool IsVisible, GameBoard* Board)
{
	GameBoardDrawer::SetDrawVisablity(IsVisible, Board);

	SetShipsVisability(IsVisible, Board);
}

void SimpleGBD::SetShipsVisability(bool IsVisible, GameBoard* Board)
{
	const std::vector<Battleship*>& Ships = Board->GetShipsOnBoard();
	for (Battleship* Ship : Ships)
	{
		Ship->GetSpriteComponent()->SetIsVisible(IsVisible);
	}
}

void SimpleGBD::SetBoardPosition(Vector2 Position, GameBoard* Board, const float CellSize)
{
	GameBoardDrawer::SetBoardPosition(Position, Board);

	const std::vector<Battleship*> ShipsOnBoard = Board->GetShipsOnBoard();
	for (Battleship* Ship : ShipsOnBoard) 
	{
		const CellCoord Coord = Ship->GetShipOnBoardCoords();
		Ship->SetPosition(Board->GetCorrectShipPosition(Ship, CellSize));
	}
}

void SimpleGBD::DrawGrid(GameBoard* Board, SDL_Renderer* Renderer, const float CellSize)
{
	CACHE_COLOR_IN();

	const Vector2 BoardPos = GetBoardPosition();
	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
	for (SDL_Rect Rect : Quads)
	{
		Rect.x = Rect.x * CellSize + BoardPos.x;
		Rect.y = Rect.y * CellSize + BoardPos.y;
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

	const Vector2 BoardPos = GetBoardPosition();
	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
	for (SDL_Rect Rect : Quads)
	{
		Rect.x = Rect.x * CellSize + BoardPos.x;
		Rect.y = Rect.y * CellSize + BoardPos.y;
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
			m_DrawHintCellHighlight && !CurrMouseOver->IsAvailableForShip() ?
			SDL_Color{ 255, 0, 0, 255 } :
			SDL_Color{ 255, 255, 0, 255 };
		SDL_SetRenderDrawColor(Renderer, DrawColor.r, DrawColor.g, DrawColor.b, DrawColor.a);

		const Vector2 BoardPos = GetBoardPosition();
		const CellCoord Coord = CurrMouseOver->GetCellCoords();
		const SDL_Rect HightlightRect = 
		{ 
			Coord.x * CellSize + BoardPos.x,
			Coord.y * CellSize + BoardPos.y,
			CellSize, 
			CellSize 
		};
		SDL_RenderDrawRect(Renderer, &HightlightRect);
	}

	CACHE_COLOR_OUT();
}

void SimpleGBD::DrawCellsStates(GameBoard* Board, SDL_Renderer* Renderer, const float CellSize)
{
	CACHE_COLOR_IN();

	const Vector2 BoardPos = GetBoardPosition();
	for (const SDL_Rect& Quad : Quads) 
	{
		const BoardCell& Cell = Board->GetCell({ (uint8_t)Quad.x, (uint8_t)Quad.y });
		const CellState State = Cell.GetCellState();
		if (State == CellState::Empty)
			continue;
		const SDL_Rect Rect =
		{
			Quad.x * CellSize + BoardPos.x,
			Quad.y * CellSize + BoardPos.y,
			Quad.w * CellSize,
			Quad.h * CellSize
		};
		switch (State)
		{
			case CellState::ShipDamaged:
				SDL_RenderCopyEx(Renderer,
					m_CellState_Damaged,
					nullptr,
					&Rect,
					0.0f,
					nullptr,
					SDL_FLIP_NONE);
				break;
			case CellState::Missed:
				SDL_SetRenderDrawColor(Renderer, 50, 50, 50, 255);
				SDL_RenderFillRect(Renderer, &Rect);
				break;
			default:
				break;
		}
	}

	CACHE_COLOR_OUT();
}

void SimpleGBD::DrawMarkupBorder(
	GameBoard* Board, SDL_Renderer* Renderer, const float CellSize)
{
	const float CellSizeHalf = CellSize / 2;
	const Vector2 BoardPos = GetBoardPosition();

	for (int i = 0; i < m_MarkupRow.size(); ++i) 
	{
		m_MarkupRow[i].DrawText(CellSize * i + CellSizeHalf + BoardPos.x, -CellSizeHalf + BoardPos.y, Renderer);
	}
	for (int i = 0; i < m_MarkupColumn.size(); ++i)
	{
		m_MarkupColumn[i].DrawText(-CellSizeHalf + BoardPos.x, CellSize * i + CellSizeHalf + BoardPos.y, Renderer);
	}
}
