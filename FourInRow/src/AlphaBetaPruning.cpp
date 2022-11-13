#include "AlphaBetaPruning.h"

#include "Board.h"
#include <algorithm>

AlphaBetaPruning::AlphaBetaPruning()
{
	
}

const BoardState* AlphaBetaPruning::AlphaBetaDecide(const BoardState* Root, int MaxDepth)
{
	const BoardState* Choise = nullptr;
	float MaxValue = -std::numeric_limits<float>::infinity();
	float Beta = -MaxValue;
	for (const BoardState* Child : Root->GetPossibleMoves(BoardState::SquareState::Red)) 
	{
		const float V = AlphaBetaMin(Child, MaxDepth - 1, MaxValue, Beta);
		if (V > MaxValue) 
		{
			MaxValue = V;
			Choise = Child;
		}
	}

	return Choise;
}

float AlphaBetaPruning::AlphaBetaMax(const BoardState* Node, int Depth, float Alpha, float Beta)
{
	if (Depth == 0 || Node->IsTerminal()) 
	{
		return Node->GetScore();
	}
	float MaxValue = -std::numeric_limits<float>::infinity();
	for (const BoardState* Child : Node->GetPossibleMoves(BoardState::SquareState::Red)) 
	{
		MaxValue = std::max(MaxValue, 
			AlphaBetaMin(Child, Depth - 1, Alpha, Beta));
		if (MaxValue >= Beta) 
		{
			return MaxValue;
		}
		Alpha = std::max(MaxValue, Alpha);
	}

	return MaxValue;
}

float AlphaBetaPruning::AlphaBetaMin(const BoardState* Node, int Depth, float Alpha, float Beta)
{
	if (Depth == 0 || Node->IsTerminal())
	{
		return Node->GetScore();
	}
	float MinValue = std::numeric_limits<float>::infinity();
	for (const BoardState* Child : Node->GetPossibleMoves(BoardState::SquareState::Yellow))
	{
		MinValue = std::min(MinValue,
			AlphaBetaMax(Child, Depth - 1, Alpha, Beta));
		if (MinValue <= Alpha)
		{
			return MinValue;
		}
		Beta = std::min(MinValue, Beta);
	}

	return MinValue;
}
