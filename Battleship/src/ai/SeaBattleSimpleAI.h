#pragma once

#include "../Actor.h"
#include "../gameboard/GameBoard.h"
#include <vector>

struct AIStep 
{
	CellCoord m_Coord;

	// Additional info


};

static bool operator==(const AIStep& Lhs, const AIStep& Rhs) 
{
	return Lhs.m_Coord == Rhs.m_Coord;
}

class SeaBattleSimpleAI : public Actor
{
public:
	SeaBattleSimpleAI(GameBoard* GameBoard, Game* GameInstance);

	void MakeMove();

private:
	GameBoard* m_TargetGameboard;

	std::vector<AIStep> m_PositiveMoves;

	std::vector<CellCoord> m_PotentialMoves;

	void ClaimCell(BoardCell& Cell);

	void ClaimCell_PostProcess(BoardCell& Cell);

	void MissedCellHandle(BoardCell& Cell);
};

