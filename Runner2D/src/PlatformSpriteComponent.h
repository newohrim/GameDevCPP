#pragma once

#include "SpriteComponent.h"
#include <vector>
#include "TextureContainer.h"

class Actor;
class Game;
struct SDL_Texture;

enum PlatformType {
	PLeft = 0,
	PMiddle = 1,
	PRight = 2
};

class PlatformTexturesContainer : public TextureContainer<SDL_Texture, PlatformType>
{
public:
	PlatformTexturesContainer(Game* GameInstance);

	virtual SDL_Texture* GetTexture(PlatformType PType) const override;

private:
	std::vector<SDL_Texture*> mPlatformTextures;
};

class PlatformSpriteComponent : public SpriteComponent
{
public:
	PlatformSpriteComponent(Actor* Owner, int PlatformLength, int DrawOrder = 100);

	virtual void Draw(SDL_Renderer* Renderer) override;

private:
	PlatformTexturesContainer PTextures;

	int mPlatformLength;
};

