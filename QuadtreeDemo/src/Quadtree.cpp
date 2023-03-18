#include "Quadtree.h"

#include <cassert>

Quadtree::Quadtree(const std::vector<float>& boundaries) : m_Root(nullptr)
{
	m_Root.boundaries = boundaries;
}

void Quadtree::AddEntity(Actor* Entity, Vector2 Position)
{
	QPartition* Partition = GetPartition(Position);
	AddEntityToPartition(Entity, Position, Partition);
}

void Quadtree::MoveEntity(Actor* Entity, Vector2 NewPos, Vector2 OldPos)
{
	QPartition* OldPartition = GetPartition(OldPos);
	QPartition* NewPartition = GetPartition(NewPos);
	if (OldPartition == NewPartition) 
	{
		UpdateEntitiesPos(Entity, NewPos, OldPartition);
		return;
	}
	
	RemoveEntityFromPartition(Entity, OldPartition);

	AddEntityToPartition(Entity, NewPos, NewPartition);

	UnifyPartitionIfPossible(OldPartition->parent);
}

void Quadtree::RemoveEntity(Actor* Entity, Vector2 Position)
{
	QPartition* Partition = GetPartition(Position);
	RemoveEntityFromPartition(Entity, Partition);
	UnifyPartitionIfPossible(Partition->parent);
}

std::vector<Actor*> Quadtree::GetEntitiesInRadius(Vector2 Position, float Radius)
{
	std::vector<Actor*> Result;
	PartitionsBypass(&m_Root, Position, Radius * Radius, Result);

	return Result;
}

typename Quadtree::QPartition* Quadtree::GetPartition(Vector2 Pos)
{
	return GetPartition(Pos, &m_Root);
}

Quadtree::QPartition* Quadtree::GetPartition(Vector2 Pos, QPartition* Partition)
{
	while (!Partition->isLeaf)
	{
		const Vector2 MedianPoint = GetMedianPoint(Partition);
		if (Pos.y <= MedianPoint.y)
		{
			if (Pos.x <= MedianPoint.x)
			{
				Partition = Partition->partitions[0];
			}
			else
			{
				Partition = Partition->partitions[1];
			}
		}
		else
		{
			if (Pos.x <= MedianPoint.x)
			{
				Partition = Partition->partitions[2];
			}
			else
			{
				Partition = Partition->partitions[3];
			}
		}
	}

	return Partition;
}

void Quadtree::DividePartition(QPartition* Partition)
{
	Partition->isLeaf = false;
	for (int i = 0; i < 4; ++i) 
	{
		Partition->partitions[i] = new QPartition(Partition);
	}

	const Vector2 MedianPoint = GetMedianPoint(Partition);

	// Set children boundaries
	Partition->partitions[0]->boundaries = 
	{ 
		Partition->boundaries[0],
		MedianPoint.x,
		Partition->boundaries[2],
		MedianPoint.y
	};
	Partition->partitions[1]->boundaries =
	{
		MedianPoint.x,
		Partition->boundaries[1],
		Partition->boundaries[2],
		MedianPoint.y
	};
	Partition->partitions[2]->boundaries =
	{
		Partition->boundaries[0],
		MedianPoint.x,
		MedianPoint.y,
		Partition->boundaries[3]
	};
	Partition->partitions[3]->boundaries =
	{
		MedianPoint.x,
		Partition->boundaries[1],
		MedianPoint.y,
		Partition->boundaries[3]
	};

	for (int i = 0; i < 4; ++i)
	{
		PopulateChildPartition(Partition->partitions[i], Partition);
	}
	// Might fix out of bounds
	if (!Partition->entities.empty()) 
	{
		for (int i = 0; i < Partition->entities.size(); ++i) 
		{
			GetPartition(Partition->entities[i].pos, Partition)->
				entities.push_back(Partition->entities[i]);
		}
		Partition->entities.clear();
	}
	assert(Partition->entities.empty());

	Partition->entities.shrink_to_fit();
}

void Quadtree::PopulateChildPartition(QPartition* Child, QPartition* Partition)
{
	for (int i = Partition->entities.size() - 1; i >= 0; --i) 
	{
		const QEntity& entity = Partition->entities[i];
		// TODO: rewrite condition?
		if (entity.pos.x > Child->boundaries[0] &&
			entity.pos.x <= Child->boundaries[1] &&
			entity.pos.y > Child->boundaries[2] &&
			entity.pos.y <= Child->boundaries[3]) 
		{
			Child->entities.push_back(entity);
			Child->size++;
			Partition->entities.erase(Partition->entities.begin() + i);
		}
	}
}

void Quadtree::AddEntityToPartition(Actor* Entity, Vector2 Position, QPartition* Partition)
{
	Partition->entities.push_back(QEntity{ Position, Entity });
	AddSizeToPartition(Partition, 1);
	if (Partition->entities.size() > MAX_ENTITIES_PER_PARTITION && Partition->level <= MAX_DIVISION_LEVEL)
	{
		DividePartition(Partition);
	}
}

void Quadtree::RemoveEntityFromPartition(Actor* Entity, QPartition* Partition)
{
	auto Iter = std::find(Partition->entities.begin(), Partition->entities.end(), Entity);
	assert(Iter != Partition->entities.end());
	Partition->entities.erase(Iter);
	AddSizeToPartition(Partition, -1);
}

void Quadtree::UpdateEntitiesPos(Actor* Entity, Vector2 Position, QPartition* Partition)
{
	auto Iter = std::find(Partition->entities.begin(), Partition->entities.end(), Entity);
	if (Iter == Partition->entities.end())
		assert(Iter != Partition->entities.end());
	Iter->pos = Position;
}

void Quadtree::UnifyPartitionIfPossible(QPartition* Partition)
{
	if (Partition && Partition->isLeaf) 
	{
		return;
	}
	while (Partition && Partition->size <= MAX_ENTITIES_PER_PARTITION) 
	{
		Partition->entities.reserve(Partition->size);
		for (int i = 0; i < 4; ++i) 
		{
			Partition->entities.insert(
				Partition->entities.end(), 
				Partition->partitions[i]->entities.begin(), 
				Partition->partitions[i]->entities.end());
			delete Partition->partitions[i];
		}

		Partition->isLeaf = true;
		Partition = Partition->parent;
	}
}

void Quadtree::AddSizeToPartition(QPartition* Partition, int Increment)
{
	while (Partition) 
	{
		Partition->size += Increment;
		Partition = Partition->parent;
	}
}

float Quadtree::GetSqDistanceToPartition(QPartition* Partition, Vector2 Point)
{
	// Source: https://stackoverflow.com/questions/5254838/calculating-distance-between-a-point-and-a-rectangular-box-nearest-point

	const Vector2 MinCorner{ Partition->boundaries[0], Partition->boundaries[2] };
	const Vector2 MaxCorner{ Partition->boundaries[1], Partition->boundaries[3] };
	const float dx = Math::Max(0.0f, 
		Math::Max(MinCorner.x - Point.x, Point.x - MaxCorner.x));
	const float dy = Math::Max(0.0f,
		Math::Max(MinCorner.y - Point.y, Point.y - MaxCorner.y));

	return dx * dx + dy * dy;
}

void Quadtree::PartitionsBypass(QPartition* Partition, Vector2 Point, float SqRadius, std::vector<Actor*>& Out)
{
	if (Partition->isLeaf) 
	{
		for (const QEntity& Entity : Partition->entities) 
		{
			if ((Entity.pos - Point).LengthSq() <= SqRadius)
			{
				Out.push_back(Entity.target);
			}
		}
		return;
	}
	for (int i = 0; i < 4; ++i) 
	{
		if (Partition->partitions[i]->size > 0 &&
			GetSqDistanceToPartition(Partition->partitions[i], Point) <= SqRadius) 
		{
			PartitionsBypass(Partition->partitions[i], Point, SqRadius, Out);
		}
	}
}

Vector2 Quadtree::GetMedianPoint(const QPartition* Partition)
{
	Vector2 MedianPoint;
	MedianPoint.x = (Partition->boundaries[1] + Partition->boundaries[0]) / 2.0f;
	MedianPoint.y = (Partition->boundaries[3] + Partition->boundaries[2]) / 2.0f;

	return MedianPoint;
}

Quadtree::QPartition::~QPartition()
{
	if (isLeaf) 
	{
		return;
	}
	for (int i = 0; i < 4; ++i) 
	{
		delete partitions[i];
	}
}
