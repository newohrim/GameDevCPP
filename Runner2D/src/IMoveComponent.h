#pragma once

#include "Math.h"

class IMoveComponent
{
public:
	virtual Vector2 GetVelocity() const = 0;
};

