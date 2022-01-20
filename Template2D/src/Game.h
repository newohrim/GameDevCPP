#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>
#include <vector>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

class DVDLogo;

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

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, SDL_Texture*> mTextures;
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class SpriteComponent*> mSprites;

	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer;
	bool mIsRunning = true;
	uint32_t mTicksCount = 0;
	bool mUpdatingActors = false;

	DVDLogo* Logo;
};

