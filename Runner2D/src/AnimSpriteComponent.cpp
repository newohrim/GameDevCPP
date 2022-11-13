#include "AnimSpriteComponent.h"

#include "SDL/SDL.h"
#include "Actor.h"

void AnimSpriteComponent::Update(float DeltaTime)
{
	SpriteComponent::Update(DeltaTime);
	if (mAnimStates.size() > 0 && mCurrAnimShot) 
	{
		SetAnimFrame(mCurrFrame + mAnimFPS * mCurrAnimShot->PlayRate * DeltaTime);
	}
}

void AnimSpriteComponent::Draw(SDL_Renderer* renderer)
{
	if (mTexture)
	{
		SDL_Rect SrcRect;
		SrcRect.w = mCurrAnimShot->SpriteWidth;
		SrcRect.h = mCurrAnimShot->SpriteHeight;
		SrcRect.x = static_cast<int>(floor(mCurrFrame) * mCurrAnimShot->SpriteWidth);
		SrcRect.y = 0;

		SDL_Rect DstRect;
		// Scale the width/height by owner's scale
		DstRect.w = static_cast<int>(mCurrAnimShot->SpriteWidth * mOwner->GetScale());
		DstRect.h = static_cast<int>(mCurrAnimShot->SpriteHeight * mOwner->GetScale());
		// Center the rectangle around the position of the owner
		DstRect.x = static_cast<int>(mOwner->GetPosition().x - DstRect.w / 2);
		DstRect.y = static_cast<int>(mOwner->GetPosition().y - DstRect.h / 2);

		// Draw (have to convert angle from radians to degrees, and clockwise to counter)
		SDL_RenderCopyEx(renderer,
			mTexture,
			&SrcRect,
			&DstRect,
			-Math::ToDegrees(mOwner->GetRotation()),
			nullptr,
			mFlipHorizontally ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
	}
}

void AnimSpriteComponent::AddAnimState(const std::string& StateName, const AnimationShot& AnimShot)
{
	SDL_assert(AnimShot.SpriteSheet);
	mAnimStates[StateName] = AnimShot;
}

bool AnimSpriteComponent::SetCurrentAnimState(const std::string& StateName)
{
	if (mCurrAnimState == StateName)
		return true;

	auto StatesIter = mAnimStates.find(StateName);
	if (StatesIter == mAnimStates.end()) 
	{
		SDL_Log("AnimSpriteComponent: AnimState %s wasn't found.", StateName);
		return false;
	}

	mCurrAnimState = StateName;
	mCurrAnimShot = &(*StatesIter).second;

	SDL_assert(mCurrAnimShot->SpriteSheet);
	SetTexture(mCurrAnimShot->SpriteSheet);
	SetAnimFrame(0.0f);

	return true;
}

void AnimSpriteComponent::SetAnimFrame(float Frame)
{
	const int UpperBound = mTexWidth / mCurrAnimShot->SpriteWidth;
	while (Frame >= UpperBound)
	{
		Frame -= UpperBound;
	}
	mCurrFrame = Frame;
}
