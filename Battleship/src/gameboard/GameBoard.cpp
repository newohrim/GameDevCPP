#include "GameBoard.h"

#include <algorithm>
#include "SimpleGBP.h"
#include "SimpleGBD.h"
#include "actors/Battleship.h"

GameBoard::GameBoard(
	const uint8_t BoardWidth,
	const uint8_t BoardHeight,
	std::unique_ptr<GameBoardDrawer> GBD,
	std::unique_ptr<GameBoardPopulator> GBP)
	: m_BoardDrawer(std::move(GBD)),
	  m_BoardPopulator(std::move(GBP))
{
	m_Grid = BoardGrid(BoardHeight, GridRow(BoardWidth));
	for (uint8_t y = 0; y < BoardHeight; ++y) 
	{
		for (uint8_t x = 0; x < BoardWidth; ++x) 
		{
			m_Grid[y][x] = BoardCell({ x, y });
		}
	}
}

GameBoard::~GameBoard()
{
	for (Battleship* Ship : m_Ships) 
	{
		Ship->DestroyDeferred();
	}
}

// TODO: Remove Pos parameter
bool GameBoard::AddShip(Battleship* Ship, const CellCoord Pos)
{
	if (!IsValidCoords(Pos) || !IsAvailableForShip(Pos, Ship))
	{
		return false;
	}

	m_Ships.push_back(Ship);
	InsertShip(Ship, Pos);
	return true;
}

bool GameBoard::IsAvailableForShip(
	const CellCoord CellCoords, 
	BattleshipStats const* ShipStats, 
	ShipOrientation const* ShipOrient)
{
	const CellCoord Direction = 
		*ShipOrient == ShipOrientation::Horizontal 
		? CellCoord{ 1, 0 } : CellCoord{ 0, 1 };
	if (!IsValidCoords(CellCoords) || 
		!IsValidCoords(CellCoords + Direction * (ShipStats->m_ShipLength - 1))) 
	{
		return false;
	}

	for (uint8_t Offset = 0; Offset < ShipStats->m_ShipLength; ++Offset) 
	{
		const CellCoord Coord = CellCoords + Direction * Offset;
		const BoardCell& Cell = GetCell(Coord);
		if (!Cell.IsEmpty() || !Cell.IsAvailableForShip())
		{
			return false;
		}
	}
	
	return true;
}

bool GameBoard::IsAvailableForShip(const CellCoord CellCoords, Battleship* Ship)
{
	ShipOrientation Orientation = Ship->GetShipOrientation();
	return IsAvailableForShip(
		CellCoords, Ship->GetShipStats(), &Orientation);
}

Vector2 GameBoard::GetCorrectShipPosition(Battleship* Ship, const float CellSize) const
{
	const float CellHalfSize = CellSize / 2.0f;
	const ShipOrientation Orientation = Ship->GetShipOrientation();
	const BattleshipStats* ShipStat = Ship->GetShipStats();
	const CellCoord ShipCoord = Ship->GetShipOnBoardCoords();
	int TexWidth, TexHeight;
	SDL_QueryTexture(ShipStat->m_ShipTexture, nullptr, nullptr, &TexWidth, &TexHeight);
	return Vector2
	{
		// H_x = x * 100 - TexWidth / 2 + length * 100 / 2.0
		(float)(ShipCoord.x * CellSize + CellHalfSize * Orientation + (ShipStat->m_ShipLength * CellSize / 2.0f) * !Orientation),
		(float)(ShipCoord.y * CellSize + CellHalfSize * !Orientation + (ShipStat->m_ShipLength * CellSize / 2.0f) * Orientation)
	} + GetBoardDrawer()->GetBoardPosition();
}

float GameBoard::GetCorrectShipRotation(Battleship* Ship) const
{
	return !Ship->GetShipOrientation() * M_PI / 2;
}

void GameBoard::InsertShip(Battleship* Ship, const CellCoord Pos)
{
	const uint8_t ShipLength = Ship->GetShipStats()->m_ShipLength;
	const CellCoord Direction = 
		Ship->GetShipOrientation() == ShipOrientation::Horizontal 
		? CellCoord{ 1, 0 } : CellCoord{ 0, 1 };
	for (uint8_t Offset = 0; Offset < ShipLength; ++Offset)
	{
		const CellCoord Coord = Pos + Direction * Offset;
		BoardCell& Cell = GetCell_Private(Coord);
		Cell.SetShip(Ship);
		MakePaddingAround(Coord);
	}
}

void GameBoard::MakePaddingAround(const CellCoord Coords)
{
	std::vector<CellCoord> PaddingAround = GetMooreNeighborhood(Coords);
	for (CellCoord C : PaddingAround) 
	{
		BoardCell& Cell = GetCell_Private(C);
		Cell.SetAvaiablityForShip(false);
	}
}

std::vector<CellCoord> GameBoard::GetMooreNeighborhood(const CellCoord Coords) const
{
	std::vector<CellCoord> Result;
	for (uint8_t x = std::max(Coords.x, (uint8_t)1U) - 1; x < std::min((uint8_t)(Coords.x + 2U), GetBoardWidth()); ++x) {
		for (uint8_t y = std::max(Coords.y, (uint8_t)1U) - 1; y < std::min((uint8_t)(Coords.y + 2U), GetBoardHeight()); ++y) {
			if (x == Coords.x && y == Coords.y) continue;
			Result.push_back({ x, y });
		}
	}

	return Result;
}

std::vector<CellCoord> GameBoard::GetCornersNeighborhood(const CellCoord Coords, int Margin) const
{
	std::vector<CellCoord> Result;
	if (Coords.x > Margin) 
	{
		if (Coords.y > Margin)
			Result.push_back(Coords - CellCoord{ 1, 1 });
		if (Coords.y < GetBoardHeight() - 1 - Margin)
			Result.push_back(Coords - CellCoord{ 1, 0 } + CellCoord{ 0, 1 });
	}
	if (Coords.x < GetBoardWidth() - 1 - Margin) 
	{
		if (Coords.y > Margin)
			Result.push_back(Coords + CellCoord{ 1, 0 } - CellCoord{ 0, 1 });
		if (Coords.y < GetBoardHeight() - 1)
			Result.push_back(Coords + CellCoord{ 1, 1 });
	}

	return Result;
}

void GameBoard::SetShipOrientation(Battleship* Ship, ShipOrientation Orientation)
{
	Ship->m_ShipOrientation = Orientation;
	// TODO: Make CellSize var in Board drawer
	Ship->SetPosition(GetCorrectShipPosition(Ship, 50.0f));
	Ship->SetRotation(GetCorrectShipRotation(Ship));
}

bool GameBoard::CheckGameOverCondition() const
{
	for (const Battleship* Ship : m_Ships) 
	{
		if (Ship->GetShipHealth() > 0) 
		{
			return false;
		}
	}

	return true;
}

std::vector<CellCoord> GameBoard::GetVonNeumannNeighborhood(const CellCoord Coords, int Margin) const
{
	std::vector<CellCoord> Result;
	if (Coords.y > Margin)
		Result.push_back(Coords - CellCoord{ 0, 1 });
	if (Coords.y < GetBoardHeight() - 1 - Margin)
		Result.push_back(Coords + CellCoord{ 0, 1 });
	if (Coords.x > Margin)
		Result.push_back(Coords - CellCoord{ 1, 0 });
	if (Coords.x < GetBoardWidth() - 1 - Margin)
		Result.push_back(Coords + CellCoord{ 1, 0 });

	return Result;
}

void BoardCell::SetCellState(CellState State)
{
	if (State == CellState::ShipDamaged &&
		m_CellState == CellState::ShipPlaced)
	{
		m_ContainedShip->TakeDamageHandle();
	}

	m_CellState = State;
}

bool GameBoard::IsPointInside(Vector2 Point) const
{
	const SDL_Point SPoint{ (int)Point.x, (int)Point.y };
	const Vector2 BoardPos = m_BoardDrawer->GetBoardPosition();
	const float CellSize = m_BoardDrawer->GetCellSize();
	const SDL_Rect BoardRect =
	{
		BoardPos.x,
		BoardPos.y,
		CellSize * GetBoardWidth(),
		CellSize * GetBoardHeight()
	};

	return SDL_PointInRect(&SPoint, &BoardRect);
}

void GameBoard::ConsumeInput_MouseOver(Vector2_Int MouseScreenCoord)
{
	// TODO
}

void GameBoard::ConsumeInput_MouseClick(Vector2_Int MouseScreenCoord)
{
	// TODO
}
