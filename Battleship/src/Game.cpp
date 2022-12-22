#include "Game.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "Random.h"
#include "Ball2D.h"
#include "CircleCollisionComponent.h"
#include "CircleRigidbodyComponent.h"
#include "gameboard/GameBoard.h"
#include "gameboard/GameBoardDrawer.h"
#include "gameboard/GameBoardPopulator.h"
#include "PlaceableBattleshipButton.h"
#include "RectangleClickZone.h"
#include "ai/SeaBattleSimpleAI.h"
#include "ui/UIInterface.h"
#include "PlacementConfirmPanel.h"

#include <iostream>

bool Game::Initialize() 
{
	const int SDLResult = SDL_Init(SDL_INIT_VIDEO);
	if (SDLResult != 0) 
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	mWindow = SDL_CreateWindow(
		"Pong", 
		100, // Top left x-coord of window
		100, // Top left y-coord of window
		1280, // Width of window
		720, // Height of window
		0); // Creating flags

	if (!mWindow) 
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		SDL_VIDEO_RENDER_OGL,      // Graphics driver
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC // Creating flags
	);

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	const int TTF_Result = TTF_Init();
	if (TTF_Result != 0)
	{
		SDL_Log("Unable to initialize TTF: %s", TTF_GetError());
		return false;
	}

	LoadData();

	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::RunLoop() 
{
	while (mIsRunning) 
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown() 
{
	UnloadData();
	IMG_Quit();
	TTF_Quit();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddActor(Actor* ActorToAdd)
{
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(ActorToAdd);
	}
	else
	{
		mActors.emplace_back(ActorToAdd);
	}
}

void Game::RemoveActor(Actor* ActorToRemove)
{
	// Is it in pending actors?
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), ActorToRemove);
	if (iter != mPendingActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// Is it in actors?
	iter = std::find(mActors.begin(), mActors.end(), ActorToRemove);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* SpriteToAdd)
{
	// Find the insertion point in the sorted vector
	// (The first element with a higher draw order than me)
	int myDrawOrder = SpriteToAdd->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	mSprites.insert(iter, SpriteToAdd);
}

void Game::RemoveSprite(SpriteComponent* SpriteToRemove)
{
	// (We can't swap because it ruins ordering)
	auto iter = std::find(mSprites.begin(), mSprites.end(), SpriteToRemove);
	mSprites.erase(iter);
}

void Game::AddUIItem(UIInterface * UIItem)
{
	m_UIItems.push_back(UIItem);
}

void Game::RemoveUIItem(UIInterface * UIItem)
{
	const auto Iter =
		std::find(m_UIItems.begin(), m_UIItems.end(), UIItem);
	if (Iter != m_UIItems.end())
	{
		m_UIItems.erase(Iter);
	}
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	// Is the texture already in the map?
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		// Load from file
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		// Create texture from surface
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}

		mTextures.emplace(fileName.c_str(), tex);
	}
	return tex;
}

void Game::ProcessInput()
{
	// Place all input devices in queue
	SDL_PumpEvents();

	// Get mouse cursor position in window space
	int Mouse_X, Mouse_Y;
	SDL_GetMouseState(&Mouse_X, &Mouse_Y);
	
	// Update highlighted cell
	if (m_GameBoard_Opponent && m_GameBoard_Opponent->GetBoardDrawer()->IsVisible())
	{
		GameBoardDrawer* BoardDrawer = m_GameBoard_Opponent->GetBoardDrawer();
		const CellCoord MouseOnBoardCoord_Opponent = BoardDrawer->
			GetWorldToBoardPos(m_GameBoard_Opponent, Vector2(Mouse_X, Mouse_Y), 50.0f);
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
			GetWorldToBoardPos(m_GameBoard_Player, Vector2(Mouse_X, Mouse_Y), 50.0f);
		if (m_GameBoard_Player->IsValidCoords(MouseOnBoardCoord_Player))
		{
			BoardCell& Cell = m_GameBoard_Player->GetCell(MouseOnBoardCoord_Player);
			if (BoardDrawer->GetCurrMouseOverCell() != &Cell) {
				BoardDrawer->SetCurrMouseOverCell(&Cell);
				RequestRedraw();
			}
		}
	}

	ProvideUIWithInput_MouseOver(
		Vector2{ (float)Mouse_X, (float)Mouse_Y });
	// TODO: Replace with event driven
	OnMouseOverHandle(Mouse_X, Mouse_Y);

	SDL_Event event;
	while (SDL_PollEvent(&event)) 
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				ProvideUIWithInput_MouseClick(
					Vector2{ (float)Mouse_X, (float)Mouse_Y });
				// TODO: Replace with event driven
				OnMouseDownHandle(Mouse_X, Mouse_Y);
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) 
				{
					case SDLK_r:
						m_PlacementShipOrientation =
							static_cast<ShipOrientation>((m_PlacementShipOrientation + 1) % 2);
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
		mIsRunning = false;
	}
}

void Game::UpdateGame() 
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	//ResolveCollisions();

	// Update all actors
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// Move any pending actors to mActors
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// Add any dead actors to a temp vector
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == ActorState::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// Delete dead actors (which removes them from mActors)
	for (auto actor : deadActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput() 
{
	if (m_RedrawRequested) 
	{
		SDL_SetRenderDrawColor(
			mRenderer,
			100,   // R
			100,   // G
			100, // B
			255  // A
		);
		SDL_RenderClear(mRenderer);

		if (m_GameBoard_Opponent)
			m_GameBoard_Opponent->GetBoardDrawer()->DrawBoard(m_GameBoard_Opponent, mRenderer, 50.0f);
		if (m_GameBoard_Player)
			m_GameBoard_Player->GetBoardDrawer()->DrawBoard(m_GameBoard_Player, mRenderer, 50.0f);

		for (SpriteComponent* sprite : mSprites)
		{
			sprite->Draw(mRenderer);
		}
		for (UIInterface* UIItem : m_UIItems) 
		{
			UIItem->DrawUI(mRenderer);
		}

		SDL_RenderPresent(mRenderer);

		m_RedrawRequested = false;
	}
}

void Game::LoadData()
{
	m_MainTextFont = TTF_OpenFont("Assets/fonts/DhasterRegular.ttf", 24);
	if (!m_MainTextFont) 
	{
		SDL_Log(TTF_GetError());
	}

	BeginGame();
}

void Game::UnloadData()
{
	delete m_GameBoard_Player;
	m_GameBoard_Player = nullptr;
	delete m_GameBoard_Opponent;
	m_GameBoard_Opponent = nullptr;

	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy textures
	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
}

void Game::BeginGame()
{
	m_ShipTamplates =
	{
		{ GetTexture("Assets/ships/carrier.png"), 5, 1 },
		{ GetTexture("Assets/ships/battleship.png"), 4, 2 },
		{ GetTexture("Assets/ships/cruiser.png"), 3, 2 },
		{ GetTexture("Assets/ships/submarine.png"), 3, 3 },
		{ GetTexture("Assets/ships/destroyer.png"), 2, 3 }
	};
	m_GameBoard_Player = new GameBoard(10, 10, this);
	m_OpponentAI = new SeaBattleSimpleAI(m_GameBoard_Player, this);
	StartPlacementStage(m_ShipTamplates);
}

void Game::GameOver(const PlayerEnum Winner)
{
	if (Winner == PlayerEnum::Player) 
	{
		std::cout << "Player won." << '\n';
	}
	else 
	{
		std::cout << "Opponent won." << '\n';
	}

	ResetGame();
	RequestRedraw();
}

void Game::ResetGame()
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

	// Reload data and recreate entities
	BeginGame();
}

void Game::ProvideUIWithInput_MouseClick(Vector2 MousePos)
{
	for (UIInterface* UIItem : m_UIItems) 
	{
		UIItem->ConsumeInput_MouseClick(MousePos);
	}
}

void Game::ProvideUIWithInput_MouseOver(Vector2 MousePos)
{
	for (UIInterface* UIItem : m_UIItems)
	{
		UIItem->ConsumeInput_MouseOver(MousePos);
	}
}

void Game::ResolveCollisions()
{
	std::vector<Vector2> NewBalls;

	for (int i = 0; i < Balls.size() - 1; ++i)
	{
		for (int j = i + 1; j < Balls.size(); ++j) 
		{
			if (CircleCollisionComponent::Intersect(
				*Balls[i]->GetCircleCollision(), *Balls[j]->GetCircleCollision())) 
			{
				Vector2 CollisionNormal =
					Balls[j]->GetCircleCollision()->GetCenter() - Balls[i]->GetCircleCollision()->GetCenter();
				CollisionNormal.Normalize();
				const Vector2 ContactPosition = 
					Balls[i]->GetCircleCollision()->GetCenter() + 
						CollisionNormal * Balls[i]->GetCircleCollision()->GetRadius();
				NewBalls.push_back(ContactPosition);
				CircleRigidbodyComponent::BounceFrom(
					Balls[i]->GetCircleRigidbody(), Balls[j]->GetCircleRigidbody(), CollisionNormal);
			}
		}
	}

	/*
	for (auto NewElement : NewBalls) 
	{
		Balls.push_back(CreateBall());
		Balls.front()->SetPosition(NewElement);
	}
	*/
}

Ball2D* Game::CreateBall()
{
	SDL_Texture* BallTexture = GetTexture(BallTexturePaths[Random::GetIntRange(0, 2)]);
	Ball2D* Ball = new Ball2D(this);
	Ball->GetSpriteComponent()->SetTexture(BallTexture);
	Ball->SetPosition(Vector2{
		Random::GetFloat() * 1024.0f,
		Random::GetFloat() * 768.0f
		});
	Ball->SetScale(0.25f);
	Ball->GetCircleCollision()->SetRadius(
		Math::Max(Ball->GetSpriteComponent()->GetTexHeight(),
			Ball->GetSpriteComponent()->GetTexWidth())
		* Ball->GetScale() / 2.0f);
	Ball->GetCircleRigidbody()->AddForce(Vector2(
		(Random::GetFloat() * 1000.0f - 500.0f) * 10.0f,
		(Random::GetFloat() * 1000.0f - 500.0f) * 10.0f
	));

	return Ball;
}

void Game::OnMouseOverHandle(const int Mouse_X, const int Mouse_Y)
{
	for (PlaceableBattleshipButton* ShipButton : m_ShipsButtons)
	{
		if (ShipButton->GetRectangleClickZone()->IsPointInsideRect(Vector2(Mouse_X, Mouse_Y)))
		{
			ShipButton->GetSpriteComponent()->SetAlphaModifier(0.5f);
			ShipButton->GetRectangleClickZone()->SetIsMouseOver(true);
			RequestRedraw();
			break;
		}
		else if (ShipButton->GetRectangleClickZone()->GetIsMouseOver())
		{
			ShipButton->GetSpriteComponent()->SetAlphaModifier(1.0f);
			ShipButton->GetRectangleClickZone()->SetIsMouseOver(false);
			RequestRedraw();
		}
	}
}

void Game::OnMouseDownHandle(const int Mouse_X, const int Mouse_Y)
{
	if (m_IsPlacementStage) 
	{
		PlacementStageClickHandle(Mouse_X, Mouse_Y);
	}
	else 
	{
		GameStageClickHandle(Mouse_X, Mouse_Y);
	}
}

void Game::StartPlacementStage(const std::vector<BattleshipStats>& Tamplates)
{
	if (m_IsPlacementStage) 
	{
		return;
	}

	m_IsPlacementStage = true;
	CreatePlacementPanel(Tamplates);
}

void Game::FinishPlacementStage()
{
	if (!m_IsPlacementStage)
	{
		return;
	}

	DestroyPlacementPanel();
	m_IsPlacementStage = false;

	// TODO: Replace opponents board creation to load game data
	m_GameBoard_Opponent = CreateAndPopulateGameboard();
	m_GameBoard_Opponent->GetBoardDrawer()->SetBoardPosition(Vector2{ 50.0f * 10 + 100.0f, 0.0f }, m_GameBoard_Opponent, 50.0f);
	m_GameBoard_Opponent->GetBoardDrawer()->SetShipsVisability(false, m_GameBoard_Opponent);
	m_PlayersTurn = PlayerEnum::Player;
	//SwitchGameboards();
}

void Game::CreatePlacementPanel(const std::vector<BattleshipStats>& Tamplates)
{
	constexpr float PlaceableShipsPanel_X = 50.0f * 12.5f;
	float PlaceableShipsPanel_Y = 0.0f;
	for (BattleshipStats& ShipTamplate : m_ShipTamplates)
	{
		m_ShipsButtons.push_back(new PlaceableBattleshipButton(
			ShipTamplate, ShipTamplate.m_ShipsBeginCount, m_MainTextFont, this));
		PlaceableShipsPanel_Y += 50.0f;
		m_ShipsButtons.back()->SetPosition(
			Vector2{ PlaceableShipsPanel_X, PlaceableShipsPanel_Y });
		m_ShipsButtons.back()->SetRotation(Math::Pi / 2.0f);
		const Vector2 Pos = 
			m_ShipsButtons.back()->GetPosition() + Vector2{ 150.0f, -25.0f };
		//m_ShipsButtons.back()->GetTextComponent()->
		//	SetTextPosition((int)Pos.x, (int)Pos.y);
	}

	RequestRedraw();
}

void Game::DestroyPlacementPanel()
{
	for (PlaceableBattleshipButton* PlacementButton : m_ShipsButtons) 
	{
		delete PlacementButton;
	}
	m_ShipsButtons.clear();

	RequestRedraw();
}

void Game::SwitchGameboards()
{
	const bool PlayersGameboardVisablity = m_GameBoard_Player->GetBoardDrawer()->IsVisible();
	m_GameBoard_Player->GetBoardDrawer()->SetDrawVisablity(!PlayersGameboardVisablity, m_GameBoard_Player);
	m_GameBoard_Opponent->GetBoardDrawer()->SetDrawVisablity(PlayersGameboardVisablity, m_GameBoard_Opponent);

	// Hide opponent's ship on Player's turn
	if (PlayersGameboardVisablity) 
	{
		m_GameBoard_Opponent->GetBoardDrawer()->SetShipsVisability(false, m_GameBoard_Opponent);
	}

	RequestRedraw();
}

void Game::PlacementStageClickHandle(const int Mouse_X, const int Mouse_Y)
{
	for (PlaceableBattleshipButton* ShipButton : m_ShipsButtons)
	{
		if (ShipButton->GetRectangleClickZone()->IsPointInsideRect(Vector2(Mouse_X, Mouse_Y)))
		{
			if (!ShipButton->IsEmpty())
			{
				// Unchoose ship tamplate if not null
				UnchooseShipTamplate();

				// Choose new one
				ChooseShipTamplate(ShipButton);
			}
			return;
		}
	}

	if (m_ChoosenShipTamplate)
	{
		// check for gameboard cell 
		GameBoardDrawer* BoardDrawer = m_GameBoard_Player->GetBoardDrawer();
		const CellCoord MouseOnBoardCoord_Player = BoardDrawer->
			GetWorldToBoardPos(m_GameBoard_Player, Vector2(Mouse_X, Mouse_Y), 50.0f);
		if (m_GameBoard_Player->IsValidCoords(MouseOnBoardCoord_Player))
		{
			BoardCell& Cell = m_GameBoard_Player->GetCell(MouseOnBoardCoord_Player);
			if (m_GameBoard_Player->IsAvailableForShip(
				MouseOnBoardCoord_Player, m_ChoosenShipTamplate->GetShipStats(), &m_PlacementShipOrientation) &&
				m_ChoosenShipTamplate->DecrementShipsCount())
			{
				// Create confirm panel
				if (!m_CreatedConfirmPanel) 
				{
					m_CreatedConfirmPanel = new PlacementConfirmPanel(&Game::ShipPlacementHandle, this);
				}
				m_CreatedConfirmPanel->SetPosition(Vector2(Mouse_X, Mouse_Y));

				// Create ship
				Battleship* Ship =
					new Battleship(m_ChoosenShipTamplate->GetShipStats(), m_PlacementShipOrientation, MouseOnBoardCoord_Player, this);
				m_GameBoard_Player->AddShip(
					Ship,
					MouseOnBoardCoord_Player);
				Ship->SetPosition(
					m_GameBoard_Player->GetCorrectShipPosition(Ship, 50.0f));
				Ship->SetRotation(
					m_GameBoard_Player->GetCorrectShipRotation(Ship));

				// Check if spare ships left to place
				bool HasSpareShips = false;
				for (PlaceableBattleshipButton* PlacementShip : m_ShipsButtons)
				{
					if (!PlacementShip->IsEmpty())
					{
						HasSpareShips = true;
						break;
					}
				}
				if (!HasSpareShips)
				{
					FinishPlacementStage();
				}
			}
		}

		// Unchoose ship tamplate (null safe)
		UnchooseShipTamplate();

		RequestRedraw();
	}
}

void Game::ChooseShipTamplate(PlaceableBattleshipButton* ShipButton)
{
	m_ChoosenShipTamplate = ShipButton;
	m_PlacementShipOrientation = ShipOrientation::Horizontal;

	// Highlight selected ship button
	m_ChoosenShipTamplate->GetSpriteComponent()->SetColorModifier({ 255, 255, 0, 255 });
	RequestRedraw();
}

void Game::UnchooseShipTamplate()
{
	if (m_ChoosenShipTamplate) 
	{
		m_ChoosenShipTamplate->GetSpriteComponent()->SetColorModifier({ 255, 255, 255, 255 });
		m_ChoosenShipTamplate = nullptr;
		RequestRedraw();
	}
}

void Game::GameStageClickHandle(const int Mouse_X, const int Mouse_Y)
{
	// check for gameboard cell 
	GameBoardDrawer* BoardDrawer = m_GameBoard_Opponent->GetBoardDrawer();
	const CellCoord MouseOnBoardCoord_Opponent = BoardDrawer->
		GetWorldToBoardPos(m_GameBoard_Opponent, Vector2(Mouse_X, Mouse_Y), 50.0f);

	if (m_GameBoard_Opponent && 
		m_GameBoard_Opponent->GetBoardDrawer()->IsVisible() &&
		m_GameBoard_Opponent->IsValidCoords(MouseOnBoardCoord_Opponent))
	{
		BoardCell& Cell = m_GameBoard_Opponent->GetCell(MouseOnBoardCoord_Opponent);
		const CellState State = Cell.GetCellState();
		switch (State)
		{
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
		m_PlayersTurn = PlayerEnum::Opponent;
		m_OpponentAI->MakeMove();
		if (m_GameBoard_Player->CheckGameOverCondition()) 
		{
			GameOver(PlayerEnum::Opponent);
			return;
		}
		m_PlayersTurn = PlayerEnum::Player;
		RequestRedraw();
	}
}

void Game::ShipPlacementHandle(bool Confirmed)
{
	if (m_CreatedConfirmPanel) 
	{
		delete(m_CreatedConfirmPanel);
		m_CreatedConfirmPanel = nullptr;
	}
}

GameBoard* Game::CreateAndPopulateGameboard()
{
	std::vector<BattleshipStats*> ShipsToSpawn;
	for (BattleshipStats& ShipTamplate : m_ShipTamplates)
	{
		ShipsToSpawn.insert(
			ShipsToSpawn.end(), ShipTamplate.m_ShipsBeginCount, &ShipTamplate);
	}
	GameBoard* ResultGB = new GameBoard(10, 10, this);
	ResultGB->GetBoardPopulator()->
		PopulateGameBoard(ResultGB, ShipsToSpawn, this, 50.0f);

	return ResultGB;
}
