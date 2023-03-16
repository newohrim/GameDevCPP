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

	void MoveEntity(Actor* Entity, Vector2 NewPos, Vector2 OldPos);

	void RemoveEntity(Actor* Entity, Vector2 Position);

public:
	struct QEntity 
	{
		Vector2 pos;
		Actor* target;

		friend bool operator==(const QEntity& lhs, const QEntity& rhs)
		{
			return lhs.target == rhs.target;
		}
		friend bool operator==(const QEntity& lhs, const Actor* rhs)
		{
			return lhs.target == rhs;
		}
	};

	struct QPartition 
	{
		QPartition(QPartition* Parent) : parent(Parent), entities(0), boundaries(0) {  }

		QPartition* parent = nullptr;
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

	inline void AddEntityToPartition(Actor* Entity, Vector2 Position, QPartition* Partition);

	inline void RemoveEntityFromPartition(Actor* Entity, QPartition* Partition);

	inline void UpdateEntitiesPos(Actor* Entity, Vector2 Position, QPartition* Partition);

	static bool IsChildOf(const QPartition* Child, const QPartition* Parent);

	static Vector2 GetMedianPoint(const QPartition* Partition);
};
