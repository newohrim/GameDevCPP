#pragma once

#include "core/Game.h"
#include "actors/Battleship.h"

class GameBoard;
class PlaceableBattleshipButton;
class SeaBattleSimpleAI;
class PlacementConfirmPanel;

enum PlayerEnum
{
	Player = 0,
	Opponent
};

enum GameState
{
	Init = 0,
	Placement,
	PlayersTurn,
	OpponentsTurn,
	GameOver
};

class SBGame : public Game
{
public:
	void ResetGame();

protected:
	virtual void ProcessInput() override;
	virtual void DrawCustom(SDL_Renderer* Renderer) override;

	virtual void LoadData() override;
	virtual void UnloadData() override;

private:
	GameBoard* m_GameBoard_Player = nullptr;
	GameBoard* m_GameBoard_Opponent = nullptr;
	std::vector<BattleshipStats> m_ShipTamplates;
	std::vector<PlaceableBattleshipButton*> m_ShipsButtons;
	SeaBattleSimpleAI* m_OpponentAI;

	GameState m_GameState = GameState::Init;
	PlaceableBattleshipButton* m_ChoosenShipTamplate = nullptr;
	ShipOrientation m_PlacementShipOrientation = ShipOrientation::Horizontal;
	TTF_Font* m_MainTextFont = nullptr;
	PlacementConfirmPanel* m_CreatedConfirmPanel = nullptr;
	Battleship* m_GhostShip = nullptr;

	void BeginGame();

	void GameOver(const PlayerEnum Winner);

	void SwitchShipOrientation();

	void CreateGameOverPanel(PlayerEnum Winner);

	void OnMouseOverHandle(const int Mouse_X, const int Mouse_Y);

	void OnMouseDownHandle(const int Mouse_X, const int Mouse_Y);

	void StartPlacementStage(const std::vector<BattleshipStats>& Tamplates);

	void FinishPlacementStage();

	void CreatePlacementPanel(const std::vector<BattleshipStats>& Tamplates);

	void DestroyPlacementPanel();

	void PlacementStageClickHandle(const int Mouse_X, const int Mouse_Y);

	void CreateConfirmPanel(const int& Mouse_X, const int& Mouse_Y);

	Battleship* CreateShip(const CellCoord& MouseOnBoardCoord_Player);

	void ChooseShipTamplate(PlaceableBattleshipButton* ShipButton);

	void UnchooseShipTamplate();

	void GameStageClickHandle(const int Mouse_X, const int Mouse_Y);

	void ShipPlacementHandle(bool Confirmed);

	void DestroyPlacementConfirmPanel();

	void DestroyGhostShip();

	GameBoard* CreateAndPopulateGameboard();
};

