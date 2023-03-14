#include "Game.h"

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "core/Actor.h"
#include "components/SpriteComponent.h"
#include "ui/UIInterface.h"

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
		1200, // Width of window
		575, // Height of window
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

void Game::AddRect2D(RectProvider2D* Rect2D)
{
	m_Rects2D.push_back(Rect2D);
}

void Game::RemoveRect2D(RectProvider2D* Rect2D)
{
	const auto Iter =
		std::find(m_Rects2D.begin(), m_Rects2D.end(), Rect2D);
	if (Iter != m_Rects2D.end())
	{
		m_Rects2D.erase(Iter);
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
	//int Mouse_X, Mouse_Y;
	SDL_GetMouseState(&m_MousePos.x, &m_MousePos.y);

	ProvideUIWithInput_MouseOver(
		Vector2(m_MousePos.x, m_MousePos.y));

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			ExitGame();
			break;
		case SDL_MOUSEBUTTONDOWN:
			ProvideUIWithInput_MouseClick(
				Vector2(m_MousePos.x, m_MousePos.y));
			break;
		default:
			break;
		}
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

		DrawCustom(mRenderer);

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

bool Game::ProvideUIWithInput_MouseClick(Vector2 MousePos)
{
	//bool WasUIInteracted = false;
	for (RectProvider2D* Rect2D : m_Rects2D) 
	{
		if (Rect2D->IsPointInside(MousePos))
		{
			Rect2D->ConsumeInput_MouseClick(
				Vector2_Int{ (int)MousePos.x, (int)MousePos.y });
			return true;
		}
	}

	return false;
}

bool Game::ProvideUIWithInput_MouseOver(Vector2 MousePos)
{
	//bool WasUIInteracted = false;
	for (RectProvider2D* Rect2D : m_Rects2D)
	{
		if (Rect2D->IsPointInside(MousePos))
		{
			Rect2D->ConsumeInput_MouseOver(
				Vector2_Int{ (int)MousePos.x, (int)MousePos.y });
			return true;
		}
	}

	return false;
}
