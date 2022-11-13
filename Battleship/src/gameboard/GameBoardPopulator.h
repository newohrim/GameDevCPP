#pragma once

#include "GameBoard.h"
#include <vector>

struct BattleshipStats;
class Game;

class GameBoardPopulator
{
public:
	virtual void PopulateGameBoard(
		GameBoard* GB, std::vector<BattleshipStats*> Templates, Game* GameInstance, const float CellSize = 100.0f) = 0;
};

