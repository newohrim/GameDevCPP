#pragma once

#include "SpriteComponent.h"
#include <vector>
#include <string>
#include <map>

class Actor;
struct SDL_Texture;

struct AnimationShot 
{
	SDL_Texture* SpriteSheet;

	int SpriteWidth = 64;

	int SpriteHeight = 64;

	float PlayRate = 1.0f;
};

class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(Actor* Owner, int DrawOrder = 150) : SpriteComponent(Owner, DrawOrder) {}

	virtual void Update(float DeltaTime) override;

	virtual void Draw(SDL_Renderer* renderer) override;

	void AddAnimState(const std::string& StateName, const AnimationShot& AnimTextures);

	std::string GetCurrentAnimState() const { return mCurrAnimState; }

	bool SetCurrentAnimState(const std::string& StateName);

	float GetAnimFPS() const { return mAnimFPS; };

	void SetAnimFPS(const float AnimFPS) { mAnimFPS = AnimFPS; };

	int GetAnimFrame() const { return static_cast<int>(mCurrFrame); }

	void SetAnimFrame(float Frame);

	void FlipHorizontally(bool DoesFlip) { mFlipHorizontally = DoesFlip; }

private:
	std::map<std::string, AnimationShot> mAnimStates;

	float mCurrFrame = 0.0f;

	float mAnimFPS = 30.0f;

	std::string mCurrAnimState;

	AnimationShot* mCurrAnimShot = nullptr;

	bool mFlipHorizontally = false;
};

