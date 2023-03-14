#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>
#include <vector>
#include "core/Math.h"

#include "SDL_ttf.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

class Actor;
class SpriteComponent;
class UIInterface;
class RectProvider2D;

class Game
{
public:
	Game() = default;
	virtual bool Initialize();
	virtual void Shutdown();
	void RunLoop();

	void AddActor(Actor* ActorToAdd);
	void RemoveActor(Actor* ActorToRemove);

	void AddSprite(SpriteComponent* SpriteToAdd);
	void RemoveSprite(SpriteComponent* SpriteToRemove);

	void AddUIItem(UIInterface* UIItem);
	void RemoveUIItem(UIInterface* UIItem);

	void AddRect2D(RectProvider2D* Rect2D);
	void RemoveRect2D(RectProvider2D* Rect2D);

	Vector2_Int GetMousePos() const { return m_MousePos; }

	SDL_Texture* GetTexture(const std::string& fileName);

	SDL_Renderer* GetRenderer() { return mRenderer; }

	void RequestRedraw() { m_RedrawRequested = true; }

protected:
	// GAME LOOP
	virtual void ProcessInput();
	virtual void UpdateGame();
	virtual void GenerateOutput();
	virtual void DrawCustom(SDL_Renderer* Renderer) {  }

	// DATA
	virtual void LoadData() = 0;
	virtual void UnloadData();

	void ExitGame() { mIsRunning = false; }

private:
	bool ProvideUIWithInput_MouseClick(Vector2 MousePos);
	bool ProvideUIWithInput_MouseOver(Vector2 MousePos);

	std::unordered_map<std::string, SDL_Texture*> mTextures;
	std::vector<Actor*> mActors;
	std::vector<Actor*> mPendingActors;
	std::vector<SpriteComponent*> mSprites;
	std::vector<UIInterface*> m_UIItems;
	std::vector<RectProvider2D*> m_Rects2D;

	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer;
	bool mIsRunning = true;
	uint32_t mTicksCount = 0;
	bool mUpdatingActors = false;
	Vector2_Int m_MousePos;
	bool m_RedrawRequested = true;
};

