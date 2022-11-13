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

#include <iostream>

bool Game::Initialize() 
{
	int SDLResult = SDL_Init(SDL_INIT_VIDEO);
	if (SDLResult != 0) 
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	mWindow = SDL_CreateWindow(
		"Pong", 
		100, // Top left x-coord of window
		100, // Top left y-coord of window
		1024, // Width of window
		768, // Height of window
		0); // Creating flags

	if (!mWindow) 
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

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
	GameBoardDrawer* BoardDrawer = m_GameBoard->GetBoardDrawer();
	const CellCoord MouseOnBoardCoord = BoardDrawer->
		GetWorldToBoardPos(m_GameBoard, Vector2(Mouse_X, Mouse_Y), 50.0f);
	if (m_GameBoard->IsValidCoords(MouseOnBoardCoord)) 
	{
		BoardCell& Cell = m_GameBoard->GetCell(MouseOnBoardCoord);
		if (BoardDrawer->GetCurrMouseOverCell() != &Cell) {
			BoardDrawer->SetCurrMouseOverCell(&Cell);
			RequestRedraw();
		}
	}

	SDL_Event event;
	while (SDL_PollEvent(&event)) 
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
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

		m_GameBoard->GetBoardDrawer()->DrawBoard(m_GameBoard, mRenderer, 50.0f);

		for (SpriteComponent* sprite : mSprites)
		{
			sprite->Draw(mRenderer);
		}

		SDL_RenderPresent(mRenderer);

		m_RedrawRequested = false;
	}
}

void Game::LoadData()
{
	ShipTamplates = 
	{
		{ GetTexture("Assets/ships/carrier.png"), 5 },
		{ GetTexture("Assets/ships/battleship.png"), 4 },
		{ GetTexture("Assets/ships/cruiser.png"), 3 },
		{ GetTexture("Assets/ships/submarine.png"), 3 },
		{ GetTexture("Assets/ships/destroyer.png"), 2 }
	};
	std::vector<BattleshipStats*> ShipsToSpawn;
	ShipsToSpawn.insert(ShipsToSpawn.end(), 1, &ShipTamplates[0]);
	ShipsToSpawn.insert(ShipsToSpawn.end(), 2, &ShipTamplates[1]);
	ShipsToSpawn.insert(ShipsToSpawn.end(), 2, &ShipTamplates[2]);
	ShipsToSpawn.insert(ShipsToSpawn.end(), 3, &ShipTamplates[3]);
	ShipsToSpawn.insert(ShipsToSpawn.end(), 3, &ShipTamplates[4]);
	m_GameBoard = new GameBoard(10, 10);
	m_GameBoard->GetBoardPopulator()->PopulateGameBoard(m_GameBoard, ShipsToSpawn, this, 50.0f);

	/*
	const int BallsNum = Random::GetIntRange(10, 100);
	Balls = std::vector<Ball2D*>(BallsNum);
	for (int i = 0; i < Balls.size(); ++i) 
	{
		Balls[i] = CreateBall();
	}
	*/

	//Logo = new DVDLogo(this);
	//Logo->SetPosition(Vector2(1024.0f / 2.0f, 768.0f / 2.0f));

}

void Game::UnloadData()
{
	delete m_GameBoard;

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
