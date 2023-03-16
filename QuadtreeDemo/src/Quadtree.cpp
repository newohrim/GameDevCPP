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
	// structure could change at this point?
	AddEntityToPartition(Entity, NewPos, NewPartition);
}

void Quadtree::RemoveEntity(Actor* Entity, Vector2 Position)
{
	QPartition* Partition = GetPartition(Position);
	RemoveEntityFromPartition(Entity, Partition);
}

typename Quadtree::QPartition* Quadtree::GetPartition(Vector2 Pos)
{
	QPartition* Partition = &m_Root;
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
			Partition->entities.erase(Partition->entities.begin() + i);
		}
	}
}

void Quadtree::AddEntityToPartition(Actor* Entity, Vector2 Position, QPartition* Partition)
{
	Partition->entities.push_back(QEntity{ Position, Entity });
	if (Partition->entities.size() > MAX_ENTITIES_PER_PARTITION)
	{
		DividePartition(Partition);
	}
}

void Quadtree::RemoveEntityFromPartition(Actor* Entity, QPartition* Partition)
{
	auto Iter = std::find(Partition->entities.begin(), Partition->entities.end(), Entity);
	assert(Iter != Partition->entities.end());
	Partition->entities.erase(Iter);

	// TODO: Add levels simplification
}

void Quadtree::UpdateEntitiesPos(Actor* Entity, Vector2 Position, QPartition* Partition)
{
	auto Iter = std::find(Partition->entities.begin(), Partition->entities.end(), Entity);
	assert(Iter != Partition->entities.end());
	Iter->pos = Position;
}

bool Quadtree::IsChildOf(const QPartition* Child, const QPartition* Parent)
{
	for (int i = 0; i < 4; ++i) 
	{
		if (Parent->partitions[i] == Child) 
		{
			return true;
		}
	}

	return false;
}

Vector2 Quadtree::GetMedianPoint(const QPartition* Partition)
{
	Vector2 MedianPoint;
	MedianPoint.x = (Partition->boundaries[1] + Partition->boundaries[0]) / 2.0f;
	MedianPoint.y = (Partition->boundaries[3] + Partition->boundaries[2]) / 2.0f;

	return MedianPoint;
}
