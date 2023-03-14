#pragma once

#include "core/Math.h"

class Game;

class RectProvider2D
{
public:
	virtual bool IsPointInside(Vector2 Point) const = 0;

	virtual void ConsumeInput_MouseOver(Vector2_Int MouseScreenCoord) {}

	virtual void ConsumeInput_MouseClick(Vector2_Int MouseScreenCoord) {}
};

