#include "QuadtreeDrawer.h"

void QuadtreeDrawer::DrawGrid(const Quadtree& QTree, SDL_Renderer* Renderer)
{
	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);

	DrawPartition(&QTree.m_Root, Renderer);
}

void QuadtreeDrawer::DrawPartition(const Quadtree::QPartition* Partition, SDL_Renderer* Renderer) const
{
	if (Partition->isLeaf) 
	{
		return;
	}

	const Vector2 MedianPoint = Quadtree::GetMedianPoint(Partition);
	SDL_RenderDrawLineF(Renderer, Partition->boundaries[0], MedianPoint.y, Partition->boundaries[1], MedianPoint.y);
	SDL_RenderDrawLineF(Renderer, MedianPoint.x, Partition->boundaries[2], MedianPoint.x, Partition->boundaries[3]);

	for (int i = 0; i < 4; ++i) 
	{
		DrawPartition(Partition->partitions[i], Renderer);
	}
}
