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
};

bool HitableList::hit(const Ray& ray, float t_min, float t_max, HitRecord& hit_record) const
{
	HitRecord temp_rec;

	bool hit_anything = false;
	float closest_so_far = t_max;

	for (auto i = 0; i < list_size; i++)
	{
		if (list[i]->hit(ray, t_min, closest_so_far, temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			hit_record = temp_rec;
		}
	}
	return hit_anything;
}
