#pragma once

#include "SDL/SDL.h"
#include "Quadtree.h"

class QuadtreeDrawer
{
public:
	void DrawGrid(const Quadtree& QTree, SDL_Renderer* Renderer);

private:
	void DrawPartition(const Quadtree::QPartition* Partition, SDL_Renderer* Renderer) const;
};

