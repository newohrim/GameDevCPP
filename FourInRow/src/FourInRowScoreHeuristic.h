#pragma once

#include <cstdint>
#include "Board.h"

using BoardStruct = int8_t**;

class FourInRowScoreHeuristic
{
public:
	FourInRowScoreHeuristic(const int BoardWidth, const int BoardHeight);

	int8_t GetHeuristic(const BoardState::SquareState** mBoard, const BoardState::SquareState PlayerTeam);

private:
	int8_t BoardWidth;
	int8_t BoardHeight;

	BoardStruct CreateEmptyBoard();
	inline void EmptyBoard(BoardStruct Board);
	inline void DeleteBoard(BoardStruct Board);
};

