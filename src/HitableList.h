#pragma once
#include "Hitable.h"
#include "HitRecord.h"

class HitableList : public Hitable
{
public:
	Hitable** list;
	int list_size;

public:
	HitableList()
	{
	}

	HitableList(Hitable** l, int num)
	{
		list = l;
		list_size = num;
	}

	bool hit(const Ray& ray, float t_min, float t_max, HitRecord& hit_record) const;
	bool bounding_box(float t0, float t1, AABB& b) const override;
};

bool HitableList::hit(const Ray& ray, float t_min, float t_max, HitRecord& hit_record) const
{
	HitRecord temp_rec;
	HitRecord saved_temp_rec;

	bool hit_anything = false;
	float closest_so_far = t_max;

	for (auto i = 0; i < list_size; i++)
	{
		if (list[i]->hit(ray, t_min, closest_so_far, temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			saved_temp_rec = temp_rec;
		}
	}
	hit_record = saved_temp_rec;
	return hit_anything;
}

inline bool HitableList::bounding_box(float t0, float t1, AABB& b) const
{
	if (list_size < 1) return false;
	AABB temp_box;
	bool first_true = list[0]->bounding_box(t0,t1,temp_box);
	if(!first_true)
		return false;
	else
		b = temp_box;
	for( int i = 1; i < list_size; i++)
	{
		if(list[0]->bounding_box(t0,t1, temp_box))
		{
			b = surrounding_box(b,temp_box);
		}
		else
			return false;
	}
	return true;
}
