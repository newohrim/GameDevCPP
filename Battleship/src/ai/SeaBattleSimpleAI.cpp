#include "SeaBattleSimpleAI.h"

#include "../Random.h"

SeaBattleSimpleAI::SeaBattleSimpleAI(GameBoard* GameBoard, Game* GameInstance) 
	: Actor(GameInstance), m_TargetGameboard(GameBoard)
{
	const int CellsCount = 
		GameBoard->GetBoardWidth() * GameBoard->GetBoardHeight();
	m_PotentialMoves = std::vector<CellCoord>(CellsCount);
	const int BoardWidth = GameBoard->GetBoardWidth();
	const int BoardHeight = GameBoard->GetBoardHeight();
	for (uint8_t i = 0; i < BoardHeight; ++i)
	{
		for (uint8_t j = 0; j < BoardWidth; ++j)
		{
			m_PotentialMoves[i * BoardWidth + j] = 
				CellCoord{ j, i };
		}
	}
}

void SeaBattleSimpleAI::MakeMove()
{
	/*
	if (!m_Moves.empty()) 
	{
		AIStep& LastStep = m_Moves.top();
		const BoardCell& Cell = m_TargetGameboard->GetCell(LastStep.m_Coord);
		if (!Cell.IsEmpty()) 
		{
			std::vector<CellCoord> Neighboors = m_TargetGameboard->GetVonNeumannNeighborhood(LastStep.m_Coord);

		}
	}
	*/

	// Pick a random cell coord mainly from m_PositiveMoves
	CellCoord PickedCoord;
	if (m_PositiveMoves.empty()) 
	{
		const int RandomIndex = 
			Random::GetIntRange(0, m_PotentialMoves.size() - 1);
		PickedCoord = m_PotentialMoves[RandomIndex];
		m_PotentialMoves.erase(m_PotentialMoves.begin() + RandomIndex);
	}
	else if (!m_PotentialMoves.empty())
	{
		const int RandomIndex =
			Random::GetIntRange(0, m_PositiveMoves.size() - 1);
		PickedCoord = m_PositiveMoves[RandomIndex].m_Coord;
		m_PositiveMoves.erase(m_PositiveMoves.begin() + RandomIndex);
	}
	else 
	{
		// No moves left
		return;
	}

	BoardCell& PickedCell = m_TargetGameboard->GetCell(PickedCoord);
	if (!PickedCell.IsEmpty() && 
		PickedCell.GetCellState() == CellState::ShipPlaced) 
	{
		ClaimCell(PickedCell);
	}
	else 
	{
		MissedCellHandle(PickedCell);
	}
}

void SeaBattleSimpleAI::ClaimCell(BoardCell& Cell)
{
	Cell.SetCellState(CellState::ShipDamaged);
	const std::vector<CellCoord> GoodMoves = 
		m_TargetGameboard->GetVonNeumannNeighborhood(Cell.GetCellCoords());
	for (const CellCoord& Coord : GoodMoves) 
	{
		// If m_PotentialMoves contains neighboor cell, then add it to m_PositiveMoves
		const auto FindResult = 
			std::find(m_PotentialMoves.begin(), m_PotentialMoves.end(), Coord);
		if (FindResult != m_PotentialMoves.end()) 
		{
			m_PositiveMoves.push_back(AIStep{ Coord });
			m_PotentialMoves.erase(FindResult);

			// Post process
			ClaimCell_PostProcess(Cell);
		}
	}
}

void SeaBattleSimpleAI::ClaimCell_PostProcess(BoardCell& Cell)
{
	const std::vector<CellCoord> BadMoves =
		m_TargetGameboard->GetCornersNeighborhood(Cell.GetCellCoords());
	for (const CellCoord& Coord : BadMoves)
	{
		// If m_PositiveMoves contains neighboor cell, then remove it
		const auto FindResult_Positive =
			std::find(m_PositiveMoves.begin(), m_PositiveMoves.end(), AIStep{ Coord });
		if (FindResult_Positive != m_PositiveMoves.end())
		{
			m_PositiveMoves.erase(FindResult_Positive);
		}
		// If m_PotentialMoves contains neighboor cell, then remove it
		const auto FindResult_Potential =
			std::find(m_PotentialMoves.begin(), m_PotentialMoves.end(), Coord);
		if (FindResult_Potential != m_PotentialMoves.end())
		{
			m_PotentialMoves.erase(FindResult_Potential);
		}
	}
}

void SeaBattleSimpleAI::MissedCellHandle(BoardCell& Cell)
{
	Cell.SetCellState(CellState::Missed);
	// Do I really need to remove neighboors???
	/*
	const std::vector<CellCoord> BadMoves =
		m_TargetGameboard->GetVonNeumannNeighborhood(Cell.GetCellCoords());
	for (const CellCoord& Coord : BadMoves)
	{
		// If m_PositiveMoves contains neighboor cell, then remove it
		const auto FindResult =
			std::find(m_PositiveMoves.begin(), m_PositiveMoves.end(), AIStep{ Coord });
		if (FindResult != m_PositiveMoves.end())
		{
			m_PositiveMoves.erase(FindResult);
		}
	}
	*/
}
