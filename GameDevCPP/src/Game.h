#pragma once

#include <cstdint>

class SDL_Window;
class SDL_Renderer;

struct Vector2
{
	float x;
	float y;
};

class Game
{
public:
	Game() = default;
	bool Initialize();
	void RunLoop();
	void Shutdown();

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer;
	bool mIsRunning = true;
	uint32_t mTicksCount = 0;
	int32_t mPaddleDir = 0;

	const int WallThickness = 15;
	const int PaddleH = 100;
	Vector2 mPaddlePos = {10.0f, 768.0f / 2.0f };
	Vector2 mBallPos = { 1024.0f / 2.0f, 768.0f / 2.0f };
	Vector2 mBallVel = { -200.0f, 235.0f };
};

