#include "FourInRowScoreHeuristic.h"

#include "Board.h"
#include <cassert>
#include <algorithm>

FourInRowScoreHeuristic::FourInRowScoreHeuristic(const int BoardWidth, const int BoardHeight)
{
	assert(BoardWidth >= 4 && BoardHeight >= 4);

	this->BoardWidth = BoardWidth;
	this->BoardHeight = BoardHeight;
}

int8_t FourInRowScoreHeuristic::GetHeuristic(const BoardState::SquareState** mBoard, const BoardState::SquareState PlayerTeam)
{
	const BoardState::SquareState OpponentTeam =
		PlayerTeam == BoardState::Red ? BoardState::Yellow : BoardState::Red;
	BoardStruct GameBoard = CreateEmptyBoard();
	for (int i = 0; i < BoardHeight; ++i)
	{
		const int RowIndex = BoardHeight - i - 1;
		bool IsNotEmptyRow = false;

		/*
		for (int j = 1; j < BoardWidth; ++j)
		{
			IsNotEmptyRow |= mBoard[RowIndex][j];
			if (mBoard[RowIndex][j - 1] == PlayerTeam)
				GameBoard[RowIndex][j]++;
		}
		for (int j = BoardWidth - 2; j >= 0; --j)
		{
			IsNotEmptyRow |= mBoard[RowIndex][j];
			if (mBoard[RowIndex][j + 1] == PlayerTeam)
				GameBoard[RowIndex][j]++;
		}
		if (!IsNotEmptyRow) 
		{
			break;
		}
		*/

		int8_t MaxScore = 0;
		int8_t CurrentScore = 0;
		int LeftBound = -1;
		for (int j = 0; j < BoardWidth; ++j) 
		{
			if (mBoard[RowIndex][j] == OpponentTeam) 
			{
				CurrentScore = 0;
				LeftBound = j;
			}
			else if (mBoard[RowIndex][j] == PlayerTeam) 
			{
				CurrentScore++;
				MaxScore = std::max(MaxScore, CurrentScore);
			}
			else if (j - LeftBound <= 2)
			{
				
			}
		}

		
	}

	DeleteBoard(GameBoard);
}

BoardStruct FourInRowScoreHeuristic::CreateEmptyBoard()
{
	BoardStruct GameBoard = new int8_t*[BoardHeight];
	EmptyBoard(GameBoard);

	return GameBoard;
}

void FourInRowScoreHeuristic::EmptyBoard(BoardStruct Board)
{
	for (int i = 0; i < BoardHeight; ++i)
	{
		Board[i] = new int8_t[BoardWidth];
		for (int j = 0; j < BoardWidth; ++j)
			Board[i][j] = 0;
	}
}

void FourInRowScoreHeuristic::DeleteBoard(BoardStruct Board)
{
	for (int i = 0; i < BoardHeight; ++i)
		delete[] Board[i];
	delete Board;
}
