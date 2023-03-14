#pragma once

#include "ui/RectProvider2D.h"
#include "core/Math.h"

struct SDL_Renderer;

class UIInterface : public RectProvider2D
{
public:
	virtual void DrawUI(SDL_Renderer* Renderer) = 0;
};

