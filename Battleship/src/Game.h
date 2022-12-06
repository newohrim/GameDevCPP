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

class Ball2D;
class GameBoard;
class PlaceableBattleshipButton;
class SeaBattleSimpleAI;

enum PlayerEnum 
{
	Player = 0,
	Opponent
};

class Game
{
public:
	Game() = default;
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* ActorToAdd);
	void RemoveActor(class Actor* ActorToRemove);

	void AddSprite(class SpriteComponent* SpriteToAdd);
	void RemoveSprite(class SpriteComponent* SpriteToRemove);

	void AddTextComponent(class TextUIComponent* TextComp);
	void RemoveTextComponent(class TextUIComponent* TextComp);

	SDL_Texture* GetTexture(const std::string& fileName);

	SDL_Renderer* GetRenderer() { return mRenderer; }

	void RequestRedraw() { m_RedrawRequested = true; }

	const std::vector<Ball2D*>& GetAllBalls() const { return Balls; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	void BeginGame();
	void GameOver(const PlayerEnum Winner);
	void ResetGame();

	void ResolveCollisions();
	Ball2D* CreateBall();

	std::unordered_map<std::string, SDL_Texture*> mTextures;
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class SpriteComponent*> mSprites;
	std::vector<class TextUIComponent*> m_TextComponents;

	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer;
	bool mIsRunning = true;
	uint32_t mTicksCount = 0;
	bool mUpdatingActors = false;

	GameBoard* m_GameBoard_Player = nullptr;
	GameBoard* m_GameBoard_Opponent = nullptr;
	PlayerEnum m_PlayersTurn = PlayerEnum::Player;
	std::vector<BattleshipStats> m_ShipTamplates;
	std::vector<PlaceableBattleshipButton*> m_ShipsButtons;
	SeaBattleSimpleAI* m_OpponentAI;

	bool m_RedrawRequested = true;
	bool m_IsPlacementStage = false;
	PlaceableBattleshipButton* m_ChoosenShipTamplate = nullptr;
	ShipOrientation m_PlacementShipOrientation = ShipOrientation::Horizontal;
	TTF_Font* m_MainTextFont = nullptr;
	
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

	void ChooseShipTamplate(PlaceableBattleshipButton* ShipButton);

	void UnchooseShipTamplate();

	void GameStageClickHandle(const int Mouse_X, const int Mouse_Y);

	GameBoard* CreateAndPopulateGameboard();
};

