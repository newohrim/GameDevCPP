#include "Quadtree.h"

Quadtree::Quadtree(const std::vector<float>& boundaries)
{
	m_Root.boundaries = boundaries;
}

void Quadtree::AddEntity(Actor* Entity, Vector2 Position)
{
	QPartition* Partition = GetPartition(Position);
	Partition->entities.push_back(QEntity{ Position, Entity });
	if (Partition->entities.size() > MAX_ENTITIES_PER_PARTITION) 
	{
		DividePartition(Partition);
	}
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
	for (int i = 0; i < 4; ++i) 
	{
		Partition->partitions[i] = new QPartition();
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
	Partition->isLeaf = false;
}

void Quadtree::PopulateChildPartition(QPartition* Child, QPartition* Partition)
{
	for (int i = Partition->entities.size() - 1; i >= 0; --i) 
	{
		const QEntity& entity = Partition->entities[i];
		if (Partition->entities[i].pos.x >= Child->boundaries[0] &&
			Partition->entities[i].pos.x < Child->boundaries[1] &&
			Partition->entities[i].pos.y >= Child->boundaries[2] &&
			Partition->entities[i].pos.y < Child->boundaries[3]) 
		{
			Child->entities.push_back(entity);
			Partition->entities.erase(Partition->entities.begin() + i);
		}
	}
}

Vector2 Quadtree::GetMedianPoint(const QPartition* Partition)
{
	Vector2 MedianPoint;
	MedianPoint.x = Partition->boundaries[0] + (Partition->boundaries[1] - Partition->boundaries[0]) / 2.0f;
	MedianPoint.y = Partition->boundaries[2] + (Partition->boundaries[3] - Partition->boundaries[2]) / 2.0f;

	return MedianPoint;
}
