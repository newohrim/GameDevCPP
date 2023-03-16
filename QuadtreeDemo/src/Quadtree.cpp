#include "Quadtree.h"

template<class T>
Quadtree<T>::Quadtree(const std::vector<float>& boundaries)
{
	m_Root.boundaries = boundaries;
}

template<class T>
void Quadtree<T>::AddEntity(T Entity, Vector2 Position)
{
	typename Quadtree<T>::QPartition* Partition = GetPartition(Position);
	Partition->entities.push_back(Entity);
	if (Partition->entities.size() > MAX_ENTITIES_PER_PARTITION) 
	{
		DividePartition(Partition);
	}
}

template<class T>
typename Quadtree<T>::QPartition* Quadtree<T>::GetPartition(Vector2 Pos) const
{
	typename Quadtree<T>::QPartition* Partition = &m_Root;
	while (!Partition->isLeaf) 
	{
		const Vector2 MedianPoint = GetMedianPoint(Partition);
		if (Pos.y <= MedianPoint.y) 
		{
			if (Pos.x <= MedianPoint.x) 
			{
				Partition = m_Root[0];
			}
			else 
			{
				Partition = m_Root[1];
			}
		}
		else 
		{
			if (Pos.x <= MedianPoint.x)
			{
				Partition = m_Root[2];
			}
			else
			{
				Partition = m_Root[3];
			}
		}
	}

	return Partition;
}

template<class T>
void Quadtree<T>::DividePartition(typename Quadtree<T>::QPartition* Partition)
{
	for (int i = 0; i < 4; ++i) 
	{
		Partition->partitions[i] = new Quadtree<T>::QPartition();
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
	Partition->partitions[2]->boundaries =
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
}

template<class T>
void Quadtree<T>::PopulateChildPartition(
	typename Quadtree<T>::QPartition* Child, typename Quadtree<T>::QPartition* Partition)
{
	for (int i = Partition->entities.size(); i >= 0; --i) 
	{
		const typename Quadtree<T>::QEntity& entity = Partition->entities[i];
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

template<class T>
Vector2 Quadtree<T>::GetMedianPoint(typename Quadtree<T>::QPartition* Partition) const
{
	Vector2 MedianPoint;
	MedianPoint.x = Partition->boundaries[0] + (Partition->boundaries[1] - Partition->boundaries[0]) / 2.0f;
	MedianPoint.y = Partition->boundaries[2] + (Partition->boundaries[3] - Partition->boundaries[2]) / 2.0f;

	return MedianPoint;
}
