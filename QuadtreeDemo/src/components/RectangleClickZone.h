#pragma once

#include "core/Component.h"
#include "core/Math.h"

class RectangleClickZone : public Component
{
public:
	RectangleClickZone(
		const int ZoneWidth, const int ZoneHeight, Actor* ComponentOwner, int UpdateOrder = 100);

	bool IsPointInsideRect(const Vector2 WorldPos) const;

	bool GetIsMouseOver() const { return m_IsMouseOver; }
	void SetIsMouseOver(bool Value) { m_IsMouseOver = Value; }

private:
	const int m_ZoneWidth;
	const int m_ZoneHeight;

	bool m_IsMouseOver = false;
};

