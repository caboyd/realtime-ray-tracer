#pragma once

#include "AABB.h"
#include "HitRecord.h"


//An abstract hittable object that is inherited so hittable objects
//can be in a list together
class Hitable
{
public:
	virtual ~Hitable() = default;

	//t_min is the minimum value along the ray that is allowed for an intersection to count.
	//negative t_min means the ray is behind the camera
	//this is the z near plane

	//t_max is the maximum value along the ray this is allowed for an intersection to count.
	//this is the z far plane
	virtual bool hit(const Ray& ray, float t_min, float t_max, HitRecord& hit_record) const = 0;
	virtual bool bounding_box(float t0, float t1, AABB& b) const = 0;
};


class FlipNormals : public Hitable
{
public:
	Hitable* ptr;

	FlipNormals(Hitable* p): ptr(p)
	{
	}

	bool hit(const Ray& ray, float t_min, float t_max, HitRecord& hit_record) const override
	{
		if (ptr->hit(ray, t_min, t_max, hit_record))
		{
			hit_record.normal = -hit_record.normal;
			return true;
		}
		else
			return false;
	}

	bool bounding_box(float t0, float t1, AABB& b) const override
	{
		return ptr->bounding_box(t0, t1, b);
	}
};

