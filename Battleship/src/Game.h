#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>
#include <vector>

#include "Battleship.h"
#include "SDL_ttf.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

class UIInterface;

class Ball2D;
class GameBoard;
class PlaceableBattleshipButton;
class SeaBattleSimpleAI;
class PlacementConfirmPanel;
class SpriteActor;

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

class Game
{
public:
	Game() = default;
	bool Initialize();
	void RunLoop();
	void Shutdown();
	void ResetGame();

	void AddActor(class Actor* ActorToAdd);
	void RemoveActor(class Actor* ActorToRemove);

	void AddSprite(class SpriteComponent* SpriteToAdd);
	void RemoveSprite(class SpriteComponent* SpriteToRemove);

	void AddUIItem(UIInterface * UIItem);
	void RemoveUIItem(UIInterface * UIItem);

	SDL_Texture* GetTexture(const std::string& fileName);

	SDL_Renderer* GetRenderer() { return mRenderer; }

	void RequestRedraw() { m_RedrawRequested = true; }

	const std::vector<Ball2D*>& GetAllBalls() const { return Balls; }

private:
	void ProcessInput();
	void SwitchShipOrientation();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void CreateGameOverPanel(PlayerEnum Winner);
	void UnloadData();

	void BeginGame();
	void GameOver(const PlayerEnum Winner);

	bool ProvideUIWithInput_MouseClick(Vector2 MousePos);
	bool ProvideUIWithInput_MouseOver(Vector2 MousePos);

	void ResolveCollisions();
	Ball2D* CreateBall();

	std::unordered_map<std::string, SDL_Texture*> mTextures;
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class SpriteComponent*> mSprites;
	std::vector<UIInterface*> m_UIItems;

	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer;
	bool mIsRunning = true;
	uint32_t mTicksCount = 0;
	bool mUpdatingActors = false;

	GameBoard* m_GameBoard_Player = nullptr;
	GameBoard* m_GameBoard_Opponent = nullptr;
	std::vector<BattleshipStats> m_ShipTamplates;
	std::vector<PlaceableBattleshipButton*> m_ShipsButtons;
	SeaBattleSimpleAI* m_OpponentAI;

	bool m_RedrawRequested = true;
	GameState m_GameState = GameState::Init;
	PlaceableBattleshipButton* m_ChoosenShipTamplate = nullptr;
	ShipOrientation m_PlacementShipOrientation = ShipOrientation::Horizontal;
	TTF_Font* m_MainTextFont = nullptr;
	PlacementConfirmPanel* m_CreatedConfirmPanel = nullptr;
	Battleship* m_GhostShip = nullptr;
	
	std::string BallTexturePaths[3] = { 
		"Assets/redball.png", 
		"Assets/blueball.png", 
		"Assets/greenball.png" 
	};
	std::vector<Ball2D*> Balls;

	void OnMouseOverHandle(const int Mouse_X, const int Mouse_Y);

	void OnMouseDownHandle(const int Mouse_X, const int Mouse_Y);

	void StartPlacementStage(const std::vector<BattleshipStats>& Tamplates);

	void FinishPlacementStage();

	void CreatePlacementPanel(const std::vector<BattleshipStats>& Tamplates);

	void DestroyPlacementPanel();

	void SwitchGameboards();

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

