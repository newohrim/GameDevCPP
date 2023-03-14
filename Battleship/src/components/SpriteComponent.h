// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once

#include "core/Component.h"
#include "interfaces/IRectProvider2D.h"
#include "SDL/SDL.h"

class SpriteComponent : public Component, public RectProvider2D
{
public:
	// (Lower draw order corresponds with further back)
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(SDL_Renderer* renderer);
	virtual void SetTexture(SDL_Texture* texture);

	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexHeight() const { return mTexHeight; }
	int GetTexWidth() const { return mTexWidth; }

	bool GetIsVisible() const { return m_IsVisible; }
	void SetIsVisible(bool IsVisible) { m_IsVisible = IsVisible; }

	float GetAlphaModifier() const { return m_AlphaModifier; }
	void SetAlphaModifier(const float AlphaMultipler) { m_AlphaModifier = AlphaMultipler; }

	const SDL_Color& GetColorModifier() const { return m_ColorModifier; }
	void SetColorModifier(const SDL_Color& ColorModifer) { m_ColorModifier = ColorModifer; }

	virtual bool IsPointInside(Vector2 Point) const override;

protected:
	SDL_Texture* mTexture;
	int mDrawOrder;
	int mTexWidth;
	int mTexHeight;
	
	float m_AlphaModifier = 1.0f;
	SDL_Color m_ColorModifier = { 255, 255, 255, 255 };

	bool m_IsVisible = true;
};

