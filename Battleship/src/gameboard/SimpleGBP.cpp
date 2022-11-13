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
			const ShipOritentation Orientation = 
				Random::GetIntRange(0, 1) == 0 ? 
				ShipOritentation::Horizontal : ShipOritentation::Vertical;
			if (GB->IsAvailableForShip(CellCoord{ x, y }, ShipStat, Orientation)) 
			{
				Battleship* Ship = new Battleship(ShipStat, Orientation, GameInstance);
				int TexWidth, TexHeight;
				SDL_QueryTexture(ShipStat->m_ShipTexture, nullptr, nullptr, &TexWidth, &TexHeight);
				Ship->SetPosition(Vector2
				{ 
					// H_x = x * 100 - TexWidth / 2 + length * 100 / 2.0
					(float)(x * CellSize + CellHalfSize * Orientation + (ShipStat->m_ShipLength * CellSize / 2.0f) * !Orientation),
					(float)(y * CellSize + CellHalfSize * !Orientation + (ShipStat->m_ShipLength * CellSize / 2.0f) * Orientation)
				});
				Ship->SetRotation(!Orientation * M_PI / 2);
				GB->AddShip(Ship, CellCoord{ x, y });
				break;
			}
		}
	}
}
