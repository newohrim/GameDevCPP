#include "SBGame.h"

#include "core/Random.h"
#include "gameboard/GameBoard.h"
#include "gameboard/SimpleGBD.h"
#include "gameboard/SimpleGBP.h"
#include "actors/PlaceableBattleshipButton.h"
#include "components/RectangleClickZone.h"
#include "ai/SeaBattleSimpleAI.h"
#include "actors/PlacementConfirmPanel.h"
#include "actors/GameOverPanel.h"
#include "actors/PlacementPanel.h"

#include "core/Actor.h"
#include "components/SpriteComponent.h"

#include <iostream>

void SBGame::ResetGame()
{
	// Wipe all entities
	delete m_GameBoard_Player;
	m_GameBoard_Player = nullptr;
	delete m_GameBoard_Opponent;
	m_GameBoard_Opponent = nullptr;
	delete m_OpponentAI;
	m_OpponentAI = nullptr;

	// Destroy panel if still exists
	DestroyPlacementPanel();

	m_GameState = GameState::Init;

	// Reload data and recreate entities
	BeginGame();
}

void SBGame::ShipTemplateSelected_Handle(BattleshipStats& Template)
{
	// Null safe calls
	DestroyGhostShip();
	DestroyPlacementConfirmPanel();

	// Unchoose ship tamplate if not null
	UnchooseShipTamplate();

	// Choose new one
	ChooseShipTamplate(&Template);

	// check for gameboard cell 
	GameBoardDrawer* const BoardDrawer = m_GameBoard_Player->GetBoardDrawer();
	const Vector2_Int MousePos = GetMousePos();
	const CellCoord MouseOnBoardCoord_Player = BoardDrawer->
		GetWorldToBoardPos(Vector2(MousePos.x, MousePos.y));
	if (m_GameBoard_Player->IsValidCoords(MouseOnBoardCoord_Player))
	{
		BoardCell& Cell = m_GameBoard_Player->GetCell(MouseOnBoardCoord_Player);
		if (m_GameBoard_Player->IsAvailableForShip(
			MouseOnBoardCoord_Player, m_ChoosenShipTamplate, &m_PlacementShipOrientation) &&
			m_PlacementPanel->GetShipsCount(m_ChoosenShipTamplate) > 0)
		{
			// Remove one ship
			m_PlacementPanel->RemoveShips(m_ChoosenShipTamplate, 1);

			// Create confirm panel
			CreateConfirmPanel(MousePos.x, MousePos.y);

			// Create ghost ship (not linked to gameboard)
			m_GhostShip = CreateShip(MouseOnBoardCoord_Player);
			m_GhostShip->GetSpriteComponent()->SetAlphaModifier(0.5f);
		}
	}

	RequestRedraw();

	RequestRedraw();
}

void SBGame::ProcessInput()
{
	Game::ProcessInput();

	Vector2_Int MousePos = GetMousePos();

	// Update highlighted cell
	if (m_GameBoard_Opponent && m_GameBoard_Opponent->GetBoardDrawer()->IsVisible())
	{
		GameBoardDrawer* BoardDrawer = m_GameBoard_Opponent->GetBoardDrawer();
		const CellCoord MouseOnBoardCoord_Opponent = BoardDrawer->
			GetWorldToBoardPos(Vector2(MousePos.x, MousePos.y));
		if (m_GameBoard_Opponent->IsValidCoords(MouseOnBoardCoord_Opponent))
		{
			BoardCell& Cell = m_GameBoard_Opponent->GetCell(MouseOnBoardCoord_Opponent);
			if (BoardDrawer->GetCurrMouseOverCell() != &Cell) {
				BoardDrawer->SetCurrMouseOverCell(&Cell);
				RequestRedraw();
			}
		}
	}
	if (m_GameBoard_Player && m_GameBoard_Player->GetBoardDrawer()->IsVisible())
	{
		GameBoardDrawer* BoardDrawer = m_GameBoard_Player->GetBoardDrawer();
		const CellCoord MouseOnBoardCoord_Player = BoardDrawer->
			GetWorldToBoardPos(Vector2(MousePos.x, MousePos.y));
		if (m_GameBoard_Player->IsValidCoords(MouseOnBoardCoord_Player))
		{
			BoardCell& Cell = m_GameBoard_Player->GetCell(MouseOnBoardCoord_Player);
			if (BoardDrawer->GetCurrMouseOverCell() != &Cell) {
				BoardDrawer->SetCurrMouseOverCell(&Cell);
				RequestRedraw();
			}
		}
	}

	/*
	if (!ProvideUIWithInput_MouseOver(
		Vector2{ (float)MousePos.x, (float)MousePos.y }))
	{
		// TODO: Replace with event driven
		OnMouseOverHandle(MousePos.x, MousePos.y);
	}
	*/

	/*
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			ExitGame();
			break;
		case SDL_MOUSEBUTTONDOWN:
			//if (!ProvideUIWithInput_MouseClick(
			//	Vector2{ (float)MousePos.x, (float)MousePos.y }))
			//{
				// TODO: Replace with event driven
			//	OnMouseDownHandle(MousePos.x, MousePos.y);
			//}
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_r:
				SwitchShipOrientation();
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		ExitGame();
	}
	*/
}

void SBGame::DrawCustom(SDL_Renderer* Renderer)
{
	if (m_GameBoard_Opponent)
		m_GameBoard_Opponent->GetBoardDrawer()->DrawBoard(m_GameBoard_Opponent, Renderer);
	if (m_GameBoard_Player)
		m_GameBoard_Player->GetBoardDrawer()->DrawBoard(m_GameBoard_Player, Renderer);
}

void SBGame::LoadData()
{
	m_MainTextFont = TTF_OpenFont("Assets/fonts/DhasterRegular.ttf", 24);
	if (!m_MainTextFont)
	{
		SDL_Log(TTF_GetError());
	}

	BeginGame();
}

void SBGame::UnloadData()
{
	delete m_GameBoard_Player;
	m_GameBoard_Player = nullptr;
	delete m_GameBoard_Opponent;
	m_GameBoard_Opponent = nullptr;

	Game::UnloadData();
}

void SBGame::BeginGame()
{
	m_ShipTamplates =
	{
		{ GetTexture("Assets/ships/carrier.png"), 5, 1 },
		{ GetTexture("Assets/ships/battleship.png"), 4, 2 },
		{ GetTexture("Assets/ships/cruiser.png"), 3, 2 },
		{ GetTexture("Assets/ships/submarine.png"), 3, 3 },
		{ GetTexture("Assets/ships/destroyer.png"), 2, 3 }
	};
	const int BoardWidth = 10;
	const int BoardHeight = 10;
	m_GameBoard_Player = new GameBoard(
		BoardWidth, 
		BoardHeight, 
		std::unique_ptr<GameBoardDrawer>(
			new SimpleGBD(BoardWidth, BoardHeight, m_CellSize, this, m_MainTextFont)), 
		std::unique_ptr<GameBoardPopulator>(
			new SimpleGBP())
	);
	m_GameBoard_Player->GetBoardDrawer()->SetBoardPosition(
		m_GameBoard_Player, Vector2{ m_CellSize, m_CellSize });
	m_OpponentAI = new SeaBattleSimpleAI(m_GameBoard_Player, this);
	StartPlacementStage(m_ShipTamplates);
}

void SBGame::GameOver(const PlayerEnum Winner)
{
	if (Winner == PlayerEnum::Player)
	{
		std::cout << "Player won." << '\n';
	}
	else
	{
		std::cout << "Opponent won." << '\n';
	}

	//ResetGame();
	CreateGameOverPanel(Winner);
	RequestRedraw();
}

void SBGame::SwitchShipOrientation()
{
	m_PlacementShipOrientation =
		static_cast<ShipOrientation>((m_PlacementShipOrientation + 1) % 2);

	if (m_GhostShip)
	{
		const bool IsAllowedToRotate = m_GameBoard_Player->IsAvailableForShip(
			m_GhostShip->GetShipOnBoardCoords(),
			m_GhostShip->GetShipStats(),
			&m_PlacementShipOrientation);
		if (IsAllowedToRotate)
		{
			m_GameBoard_Player->SetShipOrientation(m_GhostShip, m_PlacementShipOrientation);
			RequestRedraw();
		}
	}
}

void SBGame::CreateGameOverPanel(PlayerEnum Winner)
{
	GameOverPanel* Temp = new GameOverPanel(m_MainTextFont, Winner, this);
	Temp->SetPosition(Vector2{ 1200 / 2 - 200, 575 / 2 - 100 });
}

void SBGame::OnMouseOverHandle(const int Mouse_X, const int Mouse_Y)
{
	//for (PlaceableBattleshipButton* ShipButton : m_ShipsButtons)
	//{
	//	RectangleClickZone* const ClickZone = ShipButton->GetRectangleClickZone();
	//	const bool IsMouseOver_Prev = ClickZone->GetIsMouseOver();
	//	const bool IsMouseOver_New =
	//		ClickZone->IsPointInsideRect(Vector2(Mouse_X, Mouse_Y));
	//	// State transitions to avoid redraw each frame when mouse over
	//	// Mouse over state transition: false -> true
	//	if (!IsMouseOver_Prev && IsMouseOver_New)
	//	{
	//		ShipButton->GetSpriteComponent()->SetAlphaModifier(0.5f);
	//		ClickZone->SetIsMouseOver(true);
	//		RequestRedraw();
	//		break;
	//	}
	//	// true -> false
	//	else if (IsMouseOver_Prev && !IsMouseOver_New)
	//	{
	//		ShipButton->GetSpriteComponent()->SetAlphaModifier(1.0f);
	//		ClickZone->SetIsMouseOver(false);
	//		RequestRedraw();
	//	}
	//}
}

void SBGame::OnMouseDownHandle(const int Mouse_X, const int Mouse_Y)
{
	if (m_GameState == GameState::Placement)
	{
		PlacementStageClickHandle(Mouse_X, Mouse_Y);
	}
	else
	{
		GameStageClickHandle(Mouse_X, Mouse_Y);
	}
}

void SBGame::StartPlacementStage(const std::vector<BattleshipStats>& Templates)
{
	if (m_GameState == GameState::Placement)
	{
		return;
	}

	m_GameState = GameState::Placement;
	CreatePlacementPanel(Templates);
}

void SBGame::FinishPlacementStage()
{
	if (m_GameState != GameState::Placement)
	{
		return;
	}

	UnchooseShipTamplate();
	DestroyPlacementPanel();

	// TODO: Replace opponents board creation to load game data
	m_GameBoard_Opponent = CreateAndPopulateGameboard(10, 10);
	m_GameBoard_Opponent->GetBoardDrawer()->SetBoardPosition(
		m_GameBoard_Opponent, Vector2{ m_CellSize * 12 + 100.0f, m_CellSize });
	m_GameBoard_Opponent->GetBoardDrawer()->SetShipsVisability(false, m_GameBoard_Opponent);
	m_GameBoard_Opponent->GetBoardDrawer()->SetDrawUnavailableCellsHighlight(false);

	m_GameState = GameState::PlayersTurn;
}

void SBGame::CreatePlacementPanel(const std::vector<BattleshipStats>& Templates)
{
	// TODO: Check if works
	const float PlaceableShipsPanel_X = m_CellSize * 13.5f;
	const float PlaceableShipsPanel_Y = m_CellSize / 2.0f;
	m_PlacementPanel = new PlacementPanel(Templates, m_MainTextFont, this);
	m_PlacementPanel->SetPosition(Vector2(PlaceableShipsPanel_X, PlaceableShipsPanel_Y));
	
	/*
	float PlaceableShipsPanel_Y = m_CellSize / 2.0f;
	for (BattleshipStats& ShipTamplate : m_ShipTamplates)
	{
		m_ShipsButtons.push_back(new PlaceableBattleshipButton(
			ShipTamplate, ShipTamplate.m_ShipsBeginCount, m_MainTextFont, this));
		PlaceableShipsPanel_Y += m_CellSize;
		m_ShipsButtons.back()->SetPosition(
			Vector2{ PlaceableShipsPanel_X, PlaceableShipsPanel_Y });
		m_ShipsButtons.back()->SetRotation(Math::Pi / 2.0f);
		const Vector2 Pos =
			m_ShipsButtons.back()->GetPosition() + Vector2{ 1m_CellSize, -25.0f };
		//m_ShipsButtons.back()->GetTextComponent()->
		//	SetTextPosition((int)Pos.x, (int)Pos.y);
	}
	*/

	RequestRedraw();
}

void SBGame::DestroyPlacementPanel()
{
	m_PlacementPanel->DestroyDeferred();
	RequestRedraw();
}

void SBGame::PlacementStageClickHandle(const int Mouse_X, const int Mouse_Y)
{
	// Null safe calls
	DestroyGhostShip();
	DestroyPlacementConfirmPanel();

	if (m_ChoosenShipTamplate)
	{
		// check for gameboard cell 
		GameBoardDrawer* BoardDrawer = m_GameBoard_Player->GetBoardDrawer();
		const CellCoord MouseOnBoardCoord_Player = BoardDrawer->
			GetWorldToBoardPos(Vector2(Mouse_X, Mouse_Y));
		if (m_GameBoard_Player->IsValidCoords(MouseOnBoardCoord_Player))
		{
			BoardCell& Cell = m_GameBoard_Player->GetCell(MouseOnBoardCoord_Player);
			if (m_GameBoard_Player->IsAvailableForShip(
				MouseOnBoardCoord_Player, m_ChoosenShipTamplate, &m_PlacementShipOrientation) &&
				m_PlacementPanel->GetShipsCount(m_ChoosenShipTamplate) > 0)
			{
				// Remove one ship
				m_PlacementPanel->RemoveShips(m_ChoosenShipTamplate, 1);

				// Create confirm panel
				CreateConfirmPanel(Mouse_X, Mouse_Y);

				// Create ghost ship (not linked to gameboard)
				m_GhostShip = CreateShip(MouseOnBoardCoord_Player);
				m_GhostShip->GetSpriteComponent()->SetAlphaModifier(0.5f);
			}
		}

		RequestRedraw();
	}
}

void SBGame::CreateConfirmPanel(const int& Mouse_X, const int& Mouse_Y)
{
	if (!m_CreatedConfirmPanel)
	{
		m_CreatedConfirmPanel = new PlacementConfirmPanel(
			&SBGame::ShipPlacementHandle, &SBGame::SwitchShipOrientation, this);
	}
	m_CreatedConfirmPanel->SetPosition(Vector2(Mouse_X, Mouse_Y));
}

Battleship* SBGame::CreateShip(const CellCoord& MouseOnBoardCoord_Player)
{
	Battleship* Ship =
		new Battleship(m_ChoosenShipTamplate, m_PlacementShipOrientation, MouseOnBoardCoord_Player, this);
	Ship->SetPosition(
		m_GameBoard_Player->GetCorrectShipPosition(Ship, m_CellSize));
	Ship->SetRotation(
		m_GameBoard_Player->GetCorrectShipRotation(Ship));

	return Ship;
}

void SBGame::ChooseShipTamplate(BattleshipStats* Template)
{
	m_ChoosenShipTamplate = Template;
	m_PlacementShipOrientation = ShipOrientation::Horizontal;
	RequestRedraw();
}

void SBGame::UnchooseShipTamplate()
{
	if (m_ChoosenShipTamplate)
	{
		m_ChoosenShipTamplate = nullptr;
		RequestRedraw();
	}
}

void SBGame::GameStageClickHandle(const int Mouse_X, const int Mouse_Y)
{
	// check for gameboard cell 
	GameBoardDrawer* BoardDrawer = m_GameBoard_Opponent->GetBoardDrawer();
	const CellCoord MouseOnBoardCoord_Opponent = BoardDrawer->
		GetWorldToBoardPos(Vector2(Mouse_X, Mouse_Y));

	if (m_GameBoard_Opponent &&
		m_GameBoard_Opponent->GetBoardDrawer()->IsVisible() &&
		m_GameBoard_Opponent->IsValidCoords(MouseOnBoardCoord_Opponent))
	{
		BoardCell& Cell = m_GameBoard_Opponent->GetCell(MouseOnBoardCoord_Opponent);
		const CellState State = Cell.GetCellState();
		switch (State)
		{
		case CellState::Missed:
			return;
		case CellState::ShipDamaged:
			return;
		case CellState::ShipPlaced:
			Cell.SetCellState(CellState::ShipDamaged);
			if (m_GameBoard_Opponent->CheckGameOverCondition())
			{
				GameOver(PlayerEnum::Player);
				return;
			}
			break;
		case CellState::Empty:
			Cell.SetCellState(CellState::Missed);
			break;
		default:
			break;
		}
		m_GameState = GameState::OpponentsTurn;
		m_OpponentAI->MakeMove();
		if (m_GameBoard_Player->CheckGameOverCondition())
		{
			GameOver(PlayerEnum::Opponent);
			return;
		}
		m_GameState = GameState::PlayersTurn;
		RequestRedraw();
	}
}

void SBGame::ShipPlacementHandle(bool Confirmed)
{
	if (Confirmed && m_GhostShip && m_ChoosenShipTamplate && 
		m_PlacementPanel->GetShipsCount(m_ChoosenShipTamplate) > 0)
	{
		// Remove selected ship from placement panel
		m_PlacementPanel->RemoveShips(m_ChoosenShipTamplate, 1);

		// Reset ship's sprite opacity
		m_GhostShip->GetSpriteComponent()->SetAlphaModifier(1.0f);

		// Add ship on board
		m_GameBoard_Player->AddShip(
			m_GhostShip,
			m_GhostShip->GetShipOnBoardCoords());

		// Reset placement state
		m_GhostShip = nullptr;

		// Check if spare ships left to place
		if (!m_PlacementPanel->AnyShipsLeft())
		{
			FinishPlacementStage();
		}
	}
	else if (!Confirmed)
	{
		// Null safe
		DestroyGhostShip();
	}

	// Null safe
	DestroyPlacementConfirmPanel();

	// Unchoose ship tamplate (null safe)
	UnchooseShipTamplate();

	RequestRedraw();
}

void SBGame::DestroyPlacementConfirmPanel()
{
	if (m_CreatedConfirmPanel)
	{
		m_CreatedConfirmPanel->DestroyDeferred();
		m_CreatedConfirmPanel = nullptr;
	}
}

void SBGame::DestroyGhostShip()
{
	if (m_GhostShip)
	{
		m_GhostShip->DestroyDeferred();
		m_GhostShip = nullptr;
	}
}

GameBoard* SBGame::CreateAndPopulateGameboard(int BoardWidth, int BoardHeight)
{
	std::vector<BattleshipStats*> ShipsToSpawn;
	for (BattleshipStats& ShipTamplate : m_ShipTamplates)
	{
		ShipsToSpawn.insert(
			ShipsToSpawn.end(), ShipTamplate.m_ShipsBeginCount, &ShipTamplate);
	}
	GameBoard* ResultGB = new GameBoard(
		BoardWidth,
		BoardHeight,
		std::unique_ptr<GameBoardDrawer>(
			new SimpleGBD(BoardWidth, BoardHeight, m_CellSize, this, m_MainTextFont)),
		std::unique_ptr<GameBoardPopulator>(
			new SimpleGBP())
	);;
	ResultGB->GetBoardPopulator()->
		PopulateGameBoard(ResultGB, ShipsToSpawn, this, m_CellSize);

	return ResultGB;
}
