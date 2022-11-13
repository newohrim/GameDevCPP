#pragma once

#include "GameBoardPopulator.h"

class SimpleGBP : public GameBoardPopulator
{
public:
	virtual void PopulateGameBoard(
		GameBoard* GB, std::vector<BattleshipStats*> Templates, Game* GameInstance, const float CellSize = 100.0f) override;
};

