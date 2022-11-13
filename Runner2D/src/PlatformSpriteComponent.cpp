#include "PlatformSpriteComponent.h"

#include "Game.h"
#include "Actor.h"

PlatformSpriteComponent::PlatformSpriteComponent(Actor* Owner, int PlatformLength, int DrawOrder)
	: SpriteComponent(Owner, DrawOrder), PTextures(Owner->GetGame())
{
	SDL_assert(PlatformLength > 0);
	mPlatformLength = PlatformLength;
}

void PlatformSpriteComponent::Draw(SDL_Renderer* Renderer)
{
	for (int i = 0; i < mPlatformLength; ++i) 
	{
		SDL_Texture* DrawTexture = nullptr;
		if (i == 0) 
		{
			if (mPlatformLength == 1) 
			{
				DrawTexture = PTextures.GetTexture(PlatformType::PMiddle);
			}
			else 
			{
				DrawTexture = PTextures.GetTexture(PlatformType::PLeft);
			}
		}
		else if (i == mPlatformLength - 1) 
		{
			DrawTexture = PTextures.GetTexture(PlatformType::PRight);
		}
		else 
		{
			DrawTexture = PTextures.GetTexture(PlatformType::PMiddle);
		}

		SetTexture(DrawTexture);
		SDL_Rect DstRect;
		// Scale the width/height by owner's scale
		DstRect.w = static_cast<int>(mTexWidth * mOwner->GetScale());
		DstRect.h = static_cast<int>(mTexHeight * mOwner->GetScale());

		const float HorizontalOffset = mTexWidth * (i - mPlatformLength / 2);
		DstRect.x = static_cast<int>(mOwner->GetPosition().x - DstRect.w / 2) + HorizontalOffset;
		DstRect.y = static_cast<int>(mOwner->GetPosition().y - DstRect.h / 2);

		// Draw (have to convert angle from radians to degrees, and clockwise to counter)
		SDL_RenderCopyEx(Renderer,
			mTexture,
			nullptr,
			&DstRect,
			-Math::ToDegrees(mOwner->GetRotation()),
			nullptr,
			SDL_FLIP_NONE);
	}
}

PlatformTexturesContainer::PlatformTexturesContainer(Game* GameInstance)
{
	if (SDL_Texture* Texture = GameInstance->GetTexture(
		"Assets/Platforms/PlatformLeft.png")) 
	{
		mPlatformTextures.push_back(Texture);
	}
	if (SDL_Texture* Texture = GameInstance->GetTexture(
		"Assets/Platforms/PlatformMiddle.png"))
	{
		mPlatformTextures.push_back(Texture);
	}
	if (SDL_Texture* Texture = GameInstance->GetTexture(
		"Assets/Platforms/PlatformRight.png"))
	{
		mPlatformTextures.push_back(Texture);
	}
	SDL_assert(mPlatformTextures.size() == 3);
}

SDL_Texture* PlatformTexturesContainer::GetTexture(PlatformType PType) const
{
	return mPlatformTextures[static_cast<int>(PType)];
}
