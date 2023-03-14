#include "SimpleGBD.h"

#include "components/SpriteComponent.h"
#include "core/Game.h"
#include "actors/Battleship.h"

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

SimpleGBD::SimpleGBD(int Width, int Height, float CellSize, Game* GameInstance, TTF_Font* Font)
	: GameBoardDrawer(CellSize),
	  m_MarkupRow(Width), 
	  m_MarkupColumn(Height)
{
	for (uint8_t y = 0; y < Width; ++y) 
	{
		for (uint8_t x = 0; x < Height; ++x) 
		{
			Quads.push_back(SDL_Rect{ x, y, 1, 1 });
		}
	}

	char MarkupSybmol = 'A';
	for (uint8_t i = 0; i < Width; ++i)
	{
		m_MarkupRow[i] = TextUIData(Font, std::string(1, MarkupSybmol), GameInstance->GetRenderer());
		if (MarkupSybmol < 127)
			MarkupSybmol++;
	}

	for (uint8_t i = 0; i < Height; ++i)
	{
		m_MarkupColumn[i] = TextUIData(Font, std::to_string(i + 1), GameInstance->GetRenderer());
	}

	m_CellState_Damaged = 
		GameInstance->GetTexture("Assets/cells/cell_redcross.png");
}

void SimpleGBD::DrawBoard(GameBoard* Board, SDL_Renderer* Renderer)
{
	if (!IsVisible())
		return;

	DrawMarkupBorder(Renderer);
	DrawGrid(Renderer);
	DrawCellsStates(Board, Renderer);
	HighlightCurrentCell(Renderer);
}

CellCoord SimpleGBD::GetWorldToBoardPos(const Vector2 WorldPos) const
{
	const Vector2 LocalPos = WorldPos - GetBoardPosition();
	const CellCoord Result = 
	{ 
		(uint8_t)(LocalPos.x / m_CellSize),
		(uint8_t)(LocalPos.y / m_CellSize)
	};

	return Result;
}

void SimpleGBD::SetDrawVisablity(GameBoard* Board, bool IsVisible)
{
	GameBoardDrawer::SetDrawVisablity(Board, IsVisible);

	SetShipsVisability(Board, IsVisible);
}

void SimpleGBD::SetShipsVisability(GameBoard* Board, bool IsVisible)
{
	const std::vector<Battleship*>& Ships = Board->GetShipsOnBoard();
	for (Battleship* Ship : Ships)
	{
		Ship->GetSpriteComponent()->SetIsVisible(IsVisible);
	}
}

void SimpleGBD::SetBoardPosition(GameBoard* Board, Vector2 Position)
{
	GameBoardDrawer::SetBoardPosition(Board, Position);

	const std::vector<Battleship*> ShipsOnBoard = Board->GetShipsOnBoard();
	for (Battleship* Ship : ShipsOnBoard) 
	{
		const CellCoord Coord = Ship->GetShipOnBoardCoords();
		Ship->SetPosition(Board->GetCorrectShipPosition(Ship, m_CellSize));
	}
}

void SimpleGBD::DrawGrid(SDL_Renderer* Renderer)
{
	CACHE_COLOR_IN();

	const Vector2 BoardPos = GetBoardPosition();
	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
	for (SDL_Rect Rect : Quads)
	{
		Rect.x = Rect.x * m_CellSize + BoardPos.x;
		Rect.y = Rect.y * m_CellSize + BoardPos.y;
		Rect.w *= m_CellSize;
		Rect.h *= m_CellSize;
		SDL_RenderDrawRect(Renderer, &Rect);
	}
	//SDL_RenderDrawRects(Renderer, Quads.data, Quads.size());

	CACHE_COLOR_OUT();
}

void SimpleGBD::DrawCells(SDL_Renderer* Renderer)
{
	CACHE_COLOR_IN();

	const Vector2 BoardPos = GetBoardPosition();
	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
	for (SDL_Rect Rect : Quads)
	{
		Rect.x = Rect.x * m_CellSize + BoardPos.x;
		Rect.y = Rect.y * m_CellSize + BoardPos.y;
		Rect.w *= m_CellSize;
		Rect.h *= m_CellSize;
		SDL_RenderFillRect(Renderer, &Rect);
	}
	//SDL_RenderDrawRects(Renderer, Quads.data, Quads.size());

	CACHE_COLOR_OUT();
}

void SimpleGBD::HighlightCurrentCell(SDL_Renderer* Renderer)
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
			Coord.x * m_CellSize + BoardPos.x,
			Coord.y * m_CellSize + BoardPos.y,
			m_CellSize,
			m_CellSize
		};
		SDL_RenderDrawRect(Renderer, &HightlightRect);
	}

	CACHE_COLOR_OUT();
}

void SimpleGBD::DrawCellsStates(GameBoard* Board, SDL_Renderer* Renderer)
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
			Quad.x * m_CellSize + BoardPos.x,
			Quad.y * m_CellSize + BoardPos.y,
			Quad.w * m_CellSize,
			Quad.h * m_CellSize
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

void SimpleGBD::DrawMarkupBorder(SDL_Renderer* Renderer)
{
	const float CellSizeHalf = m_CellSize / 2;
	const Vector2 BoardPos = GetBoardPosition();

	for (int i = 0; i < m_MarkupRow.size(); ++i) 
	{
		m_MarkupRow[i].DrawText(m_CellSize * i + CellSizeHalf + BoardPos.x, -CellSizeHalf + BoardPos.y, Renderer);
	}
	for (int i = 0; i < m_MarkupColumn.size(); ++i)
	{
		m_MarkupColumn[i].DrawText(-CellSizeHalf + BoardPos.x, m_CellSize * i + CellSizeHalf + BoardPos.y, Renderer);
	}
}
