#include "Game.h"
#include "SDL/SDL.h"

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
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
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

	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddleDir += 1;
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

	if (mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
	}
	if (mPaddlePos.y < (PaddleH / 2.0f + WallThickness))
	{
		mPaddlePos.y = PaddleH / 2.0f + WallThickness;
	}
	else if (mPaddlePos.y > (768.0f - PaddleH / 2.0f - WallThickness))
	{
		mPaddlePos.y = 768.0f - PaddleH / 2.0f - WallThickness;
	}

	// Update ball position
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	// Check collision
	// Bounce if needed
	// Did we intersect with the paddle?
	float diff = mPaddlePos.y - mBallPos.y;
	// Take absolute value of difference
	diff = (diff > 0.0f) ? diff : -diff;
	if (
		// Our y-difference is small enough
		diff <= PaddleH / 2.0f &&
		// We are in the correct x-position
		mBallPos.x <= 25.0f &&
		// The ball is moving to the left
		mBallVel.x < 0.0f)
	{
		mBallVel.x *= -1.0f;
	}
	// Did the ball go off the screen? (if so, end game)
	else if (mBallPos.x <= 0.0f)
	{
		mIsRunning = false;
	}
	// Did the ball collide with the right wall?
	else if (mBallPos.x >= (1024.0f - WallThickness) && mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1.0f;
	}

	// Did the ball collide with the top wall?
	if (mBallPos.y <= WallThickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1;
	}
	// Did the ball collide with the bottom wall?
	else if (mBallPos.y >= (768 - WallThickness) &&
		mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1;
	}
}

void Game::GenerateOutput() 
{
	SDL_SetRenderDrawColor(
		mRenderer,
		0,   // R
		0,   // G
		255, // B
		255  // A
	);
	SDL_RenderClear(mRenderer);

	SDL_Rect UpperWall{
		0,        // Top left x
		0,        // Top left y
		1024,     // Width
		WallThickness // Height
	};
	SDL_Rect RightWall{
		1024 - WallThickness,        // Top left x
		0,        // Top left y
		WallThickness,     // Width
		768 // Height
	};
	SDL_Rect LowerWall{
		0,        // Top left x
		768 - WallThickness,        // Top left y
		1024,     // Width
		WallThickness // Height
	};
	SDL_Rect Paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - PaddleH / 2),
		WallThickness,
		PaddleH
	};
	SDL_Rect Ball{
		static_cast<int>(mBallPos.x - WallThickness / 2),
		static_cast<int>(mBallPos.y - WallThickness / 2),
		WallThickness,
		WallThickness
	};

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_RenderFillRect(mRenderer, &UpperWall);
	SDL_RenderFillRect(mRenderer, &RightWall);
	SDL_RenderFillRect(mRenderer, &LowerWall);
	SDL_RenderFillRect(mRenderer, &Paddle);
	SDL_RenderFillRect(mRenderer, &Ball);

	SDL_RenderPresent(mRenderer);
}