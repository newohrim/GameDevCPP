#pragma once

#include "core/Math.h"
#include <vector>

template<class T>
class Quadtree
{
public:
	Quadtree(const std::vector<float>& boundaries);

	void AddEntity(T Entity, Vector2 Position);

public:
	struct QEntity;

	struct QPartition;

private:
	QPartition m_Root;
	static constexpr int MAX_ENTITIES_PER_PARTITION = 8;

	typename Quadtree<T>::QPartition* GetPartition(Vector2 Pos) const;

	void DividePartition(QPartition* Partition);

	void PopulateChildPartition(QPartition* Child, QPartition* Parent);

	Vector2 GetMedianPoint(QPartition* Partition) const;
};

template<class T>
struct Quadtree<T>::QEntity
{
	Vector2 pos;
	T target;
};

template<class T>
struct Quadtree<T>::QPartition
{
	QPartition() : entities(0), boundaries(0) {  }

	QPartition* partitions[4];
	std::vector<QEntity> entities;
	std::vector<float> boundaries;
	bool isLeaf = true;
};
