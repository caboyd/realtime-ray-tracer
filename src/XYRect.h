#pragma once
#include "Hitable.h"
#include "Material.h"


class XYRect : public Hitable
{
	float x0, x1, y0, y1, k;
	Material* mp;
public:
	XYRect()
	{
	};

	XYRect(float _x0, float _x1, float _y0, float _y1, float _k, Material* mat) : x0(_x0),
	                                                                              x1(_x1), y0(_y0), y1(_y1), k(_k),
	                                                                              mp(mat)
	{
	};


	bool hit(const Ray& ray, float t_min, float t_max, HitRecord& hit_record) const override
	{
		float t = (k - ray.origin.z) / ray.direction.z;

		if (t < t_min || t > t_max)
			return false;

		float x = ray.origin.x + t * ray.direction.x;
		float y = ray.origin.y + t * ray.direction.y;

		if (x < x0 || x > x1 || y < y0 || y > y1)
			return false;

		hit_record.u = (x - x0) / (x1 - x0);
		hit_record.v = (y - y0) / (y1 - y0);
		hit_record.t = t;
		hit_record.mat_ptr = mp;
		hit_record.position = ray.point_at_parameter(t);
		hit_record.normal = Vector3::UNIT_Z_POS;
		return true;
	}

	bool bounding_box(float t0, float t1, AABB& b) const override
	{
		b = AABB({x0, y0, k - 0.0001f}, {x1, y1, k + 0.0001f});
		return true;
	}
};

class XZRect : public Hitable
{
	float x0, x1, z0, z1, k;
	Material* mp;
public:
	XZRect()
	{
	};

	XZRect(float _x0, float _x1, float _z0, float _z1, float _k, Material* mat) : x0(_x0),
	                                                                              x1(_x1), z0(_z0), z1(_z1), k(_k),
	                                                                              mp(mat)
	{
	};


	bool bounding_box(float t0, float t1, AABB& b) const override
	{
		b = AABB({x0, k - 0.0001f, z0}, {x1, k + 0.0001f, z1});
		return true;
	}

	bool hit(const Ray& ray, float t_min, float t_max, HitRecord& hit_record) const override
	{
		float t = (k - ray.origin.y) / ray.direction.y;

		if (t < t_min || t > t_max)
			return false;

		float x = ray.origin.x + t * ray.direction.x;
		float z = ray.origin.z + t * ray.direction.z;

		if (x < x0 || x > x1 || z < z0 || z > z1)
			return false;

		hit_record.u = (x - x0) / (x1 - x0);
		hit_record.v = (z - z0) / (z1 - z0);
		hit_record.t = t;
		hit_record.mat_ptr = mp;
		hit_record.position = ray.point_at_parameter(t);
		hit_record.normal = Vector3::UNIT_Y_POS;
		return true;
	}
};


class YZRect : public Hitable
{
	float y0, y1, z0, z1, k;
	Material* mp;
public:
	YZRect()
	{
	};

	YZRect(float _y0, float _y1, float _z0, float _z1, float _k, Material* mat) : y0(_y0),
	                                                                              y1(_y1), z0(_z0), z1(_z1), k(_k),
	                                                                              mp(mat)
	{
	};


	bool bounding_box(float t0, float t1, AABB& b) const override
	{
		b = AABB({k - 0.0001f, y0, z0}, {k + 0.0001f, y1, z1});
		return true;
	}

	bool hit(const Ray& ray, float t_min, float t_max, HitRecord& hit_record) const override
	{
		float t = (k - ray.origin.x) / ray.direction.x;

		if (t < t_min || t > t_max)
			return false;

		float y = ray.origin.y + t * ray.direction.y;
		float z = ray.origin.z + t * ray.direction.z;

		if (y < y0 || y > y1 || z < z0 || z > z1)
			return false;

		hit_record.u = (y - y0) / (y1 - y0);
		hit_record.v = (z - z0) / (z1 - z0);
		hit_record.t = t;
		hit_record.mat_ptr = mp;
		hit_record.position = ray.point_at_parameter(t);
		hit_record.normal = Vector3::UNIT_X_POS;
		return true;
	}
};


