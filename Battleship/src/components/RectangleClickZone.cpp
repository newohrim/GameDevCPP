#include "RectangleClickZone.h"

#include "SDL/SDL.h"
#include "core/Actor.h"

#include "core/Game.h"

RectangleClickZone::RectangleClickZone(
	const int ZoneWidth, const int ZoneHeight, Actor* ComponentOwner, int UpdateOrder)
	: Component(ComponentOwner, UpdateOrder), m_ZoneWidth(ZoneWidth), m_ZoneHeight(ZoneHeight)
{
	// PLACEHOLDER
}

bool RectangleClickZone::IsPointInsideRect(const Vector2 WorldPos) const
{
	const SDL_Point Point = { WorldPos.x, WorldPos.y };
	const Vector2 OwnerPosition = mOwner->GetPosition();
	const SDL_Rect Rect = 
	{ 
		OwnerPosition.x - m_ZoneWidth / 2, 
		OwnerPosition.y - m_ZoneHeight / 2, 
		m_ZoneWidth, 
		m_ZoneHeight 
	};
	return SDL_PointInRect(&Point, &Rect);
}
