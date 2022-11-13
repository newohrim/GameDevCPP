#include "Game.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "PlayerCharacter.h"
#include "Platform.h"
#include "CharacterMoveComponent.h"
#include "BoxCollisionComponent.h"

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

	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,      // Graphics driver
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

void Game::AddCollision(BoxCollisionComponent* Collision)
{
	mCollisions.push_back(Collision);
}

std::vector<BoxCollisionComponent*> Game::GetOverlappingCollisions(const BoxCollisionComponent* Collision) const
{
	std::vector<BoxCollisionComponent*> Result;

	for (BoxCollisionComponent* OtherBox : mCollisions)
	{
		if (Collision != OtherBox && BoxCollisionComponent::AABB(Collision, OtherBox))
		{
			Result.push_back(OtherBox);
		}
	}

	return Result;
}

void Game::ProcessInput()
{
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

	CharacterMoveComponent* MoveComponent = PlayerChar->GetMoveComponent();
	SDL_assert(MoveComponent);
	if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]) 
	{
		MoveComponent->AddVelocity(Vector2(1.0f, 0.0f));
	}
	if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])
	{
		MoveComponent->AddVelocity(Vector2(-1.0f, 0.0f));
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
	SDL_SetRenderDrawColor(
		mRenderer,
		255,   // R
		255,   // G
		255, // B
		255  // A
	);
	SDL_RenderClear(mRenderer);

	for (SpriteComponent* sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}

	for (BoxCollisionComponent* Box : mCollisions) 
	{
		SDL_SetRenderDrawColor(
			mRenderer,
			0,   // R
			255,   // G
			0, // B
			255  // A
		);
		for (BoxCollisionComponent* OtherBox : mCollisions) 
		{
			if (Box != OtherBox && BoxCollisionComponent::AABB(Box, OtherBox)) 
			{
				SDL_SetRenderDrawColor(
					mRenderer,
					255,   // R
					0,   // G
					0, // B
					255  // A
				);
			}
		}
		SDL_RenderDrawRect(mRenderer, &Box->GetCollisionRect_World());
	}

	SDL_RenderPresent(mRenderer);
}

void Game::LoadData()
{
	PlayerChar = new PlayerCharacter(this);
	PlayerChar->SetPosition(Vector2(1024.0f / 2.0f, 768.0f / 2.0f - 100.0f));
	PlayerChar->SetScale(2.0f);

	TestPlatform = new Platform(this, 5);
	TestPlatform->SetPosition(Vector2(1024.0f / 2.0f, 768.0f / 2.0f + 72.0f));
	TestPlatform->SetScale(1.0f);
}

void Game::UnloadData()
{
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
