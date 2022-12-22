#pragma once

#include "../Math.h"

struct SDL_Renderer;

class UIInterface
{
public:
	virtual void DrawUI(SDL_Renderer* Renderer) = 0;

	virtual bool IsPointInside(Vector2 Point) = 0;

	virtual void ConsumeInput_MouseClick(Vector2 MousePos) = 0;

	virtual void ConsumeInput_MouseOver(Vector2 MousePos) = 0;
};

