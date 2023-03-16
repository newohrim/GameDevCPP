#pragma once

#include "core/Math.h"
#include <vector>

class Actor;

class Quadtree
{

public:
	friend class QuadtreeDrawer;

	Quadtree(const std::vector<float>& boundaries);

	void AddEntity(Actor* Entity, Vector2 Position);

public:
	struct QEntity 
	{
		Vector2 pos;
		Actor* target;
	};

	struct QPartition 
	{
		QPartition() : entities(0), boundaries(0) {  }

		QPartition* partitions[4];
		std::vector<QEntity> entities;
		std::vector<float> boundaries;
		bool isLeaf = true;
	};

private:
	static constexpr int MAX_ENTITIES_PER_PARTITION = 8;

	QPartition m_Root;

	QPartition* GetPartition(Vector2 Pos);

	void DividePartition(QPartition* Partition);

	void PopulateChildPartition(QPartition* Child, QPartition* Parent);

	static Vector2 GetMedianPoint(const QPartition* Partition);
};
