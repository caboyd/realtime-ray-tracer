#pragma once
#include "Hitable.h"
#include "AABB.h"
#include "HitRecord.h"
#include "Random.h"
#include <iostream>

int box_x_compare(const void* a, const void* b);
int box_y_compare(const void* a, const void* b);
int box_z_compare(const void* a, const void* b);

class BVHNode : public Hitable
{
public:

	Hitable* left;
	Hitable* right;
	AABB box;


	BVHNode()
	{
	};
	BVHNode(Hitable** l, int n, float time0, float time1);

	bool hit(const Ray& ray, float t_min, float t_max, HitRecord& hit_record) const override;
	bool bounding_box(float t0, float t1, AABB& b) const override;
};

inline BVHNode::BVHNode(Hitable** l, int n, float time0, float time1)
{
	int axis = int(Random::randi(3));
	if (axis == 0)
		qsort(l, n, sizeof(Hitable *), box_x_compare);
	else if (axis == 1)
		qsort(l, n, sizeof(Hitable *), box_y_compare);
	else
		qsort(l, n, sizeof(Hitable *), box_z_compare);
	if (n == 1)
	{
		left = right = l[0];
	}
	else if (n == 2)
	{
		left = l[0];
		right = l[1];
	}
	else
	{
		left = new BVHNode(l, n / 2, time0, time1);
		right = new BVHNode(l + n / 2, n - n / 2, time0, time1);
	}
	AABB box_left, box_right;
	if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right))
		std::cerr << "no bounding box in BVHNode constructor\n";
	box = surrounding_box(box_left,box_right);
}


inline bool BVHNode::bounding_box(float t0, float t1, AABB& b) const
{
	b = box;
	return true;
}

inline bool BVHNode::hit(const Ray& ray, float t_min, float t_max, HitRecord& hit_record) const
{
	if (box.hit(ray, t_min, t_max))
	{
		HitRecord left_rec, right_rec;
		bool hit_left = left->hit(ray, t_min, t_max, left_rec);
		bool hit_right = right->hit(ray, t_min, t_max, right_rec);
		if (hit_left && hit_right)
		{
			if (left_rec.t < right_rec.t)
				hit_record = left_rec;
			else
				hit_record = right_rec;
			return true;
		}
		else if (hit_left)
		{
			hit_record = left_rec;
			return true;
		}
		else if (hit_right)
		{
			hit_record = right_rec;
			return true;
		}
		else
			return false;
	}
	else return false;
}


inline int box_x_compare(const void* a, const void* b)
{
	AABB box_left, box_right;
	Hitable* ah = *(Hitable **)a;
	Hitable* bh = *(Hitable **)b;
	if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
		std::cerr << "no bounding box in BVHNode Constructor\n";
	if (box_left.min.x - box_right.min.x < 0.0)
		return -1;
	return 1;
}

inline int box_y_compare(const void* a, const void* b)
{
	AABB box_left, box_right;
	Hitable* ah = *(Hitable **)a;
	Hitable* bh = *(Hitable **)b;
	if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
		std::cerr << "no bounding box in BVHNode Constructor\n";
	if (box_left.min.y - box_right.min.y < 0.0)
		return -1;
	return 1;
}

inline int box_z_compare(const void* a, const void* b)
{
	AABB box_left, box_right;
	Hitable* ah = *(Hitable **)a;
	Hitable* bh = *(Hitable **)b;
	if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
		std::cerr << "no bounding box in BVHNode Constructor\n";
	if (box_left.min.z - box_right.min.z < 0.0)
		return -1;
	return 1;
}
