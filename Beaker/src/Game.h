#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>
#include <vector>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

class Ball2D;

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

	SDL_Texture* GetTexture(const std::string& fileName);

	const std::vector<Ball2D*>& GetAllBalls() const { return Balls; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	void ResolveCollisions();
	Ball2D* CreateBall();

	std::unordered_map<std::string, SDL_Texture*> mTextures;
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class SpriteComponent*> mSprites;

	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer;
	bool mIsRunning = true;
	uint32_t mTicksCount = 0;
	bool mUpdatingActors = false;

	std::string BallTexturePaths[3] = { 
		"Assets/redball.png", 
		"Assets/blueball.png", 
		"Assets/greenball.png" 
	};
	std::vector<Ball2D*> Balls;
};

