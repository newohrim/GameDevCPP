#include "SimpleGBP.h"

#include "../Random.h"
#include "../Battleship.h"
#include "../Game.h"

#include "SDL/SDL.h"

void SimpleGBP::PopulateGameBoard(
	GameBoard* GB, std::vector<BattleshipStats*> Templates, Game* GameInstance, const float CellSize)
{
	const float CellHalfSize = CellSize / 2.0f;
	for (BattleshipStats* ShipStat : Templates) 
	{
		int Iter = 1000;
		while (Iter-- > 0) 
		{
			const uint8_t x =
				Random::GetIntRange(0, GB->GetBoardWidth() - 1);
			const uint8_t y =
				Random::GetIntRange(0, GB->GetBoardHeight() - 1);
			const ShipOrientation Orientation = 
				Random::GetIntRange(0, 1) == 0 ? 
				ShipOrientation::Horizontal : ShipOrientation::Vertical;
			if (GB->IsAvailableForShip(CellCoord{ x, y }, ShipStat, &Orientation)) 
			{
				Battleship* Ship = new Battleship(ShipStat, Orientation, CellCoord{ x, y }, GameInstance);
				int TexWidth, TexHeight;
				SDL_QueryTexture(ShipStat->m_ShipTexture, nullptr, nullptr, &TexWidth, &TexHeight);
				
				Ship->SetPosition(GB->GetCorrectShipPosition(Ship, CellSize));
				Ship->SetRotation(GB->GetCorrectShipRotation(Ship));
				GB->AddShip(Ship, CellCoord{ x, y });
				break;
			}
		}
	}
}
