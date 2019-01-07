#pragma once
#include "Hitable.h"
#include "Material.h"


class XYRect : public Hitable
{
	float x0, x1, y0, y1, k;
	Material* mp;
	bool flip_normals;
public:
	XYRect()
	{
	};

	XYRect(float _x0, float _x1, float _y0, float _y1, float _k, Material* mat, bool flip_normals = false) : x0(_x0),
	                                                                                                         x1(_x1),
	                                                                                                         y0(_y0),
	                                                                                                         y1(_y1),
	                                                                                                         k(_k),
	                                                                                                         mp(mat),
	                                                                                                         flip_normals(
		                                                                                                         flip_normals)
	{
	};


	bool hit(const Ray& r, float t_min, float t_max, HitRecord& hit_record) const override
	{
		Ray ray = r;
		float t = (k - ray.origin.z) / ray.direction.z;

		if (t < t_min || t > t_max)
			return false;

		float x = ray.origin.x + t * ray.direction.x;
		float y = ray.origin.y + t * ray.direction.y;

		if (x < x0 || x > x1 || y < y0 || y > y1)
			return false;

		HitRecord temp_rec;
		temp_rec.u = (x - x0) / (x1 - x0);
		temp_rec.v = (y - y0) / (y1 - y0);
		temp_rec.t = t;
		temp_rec.mat_ptr = mp;
		temp_rec.position = ray.point_at_parameter(t);
		temp_rec.normal = flip_normals ? Vector3::UNIT_Z_NEG : Vector3::UNIT_Z_POS;
		hit_record = temp_rec;
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
	bool flip_normals;
public:
	XZRect()
	{
	};

	XZRect(float _x0, float _x1, float _z0, float _z1, float _k, Material* mat, bool flip_normals = false) : x0(_x0),
	                                                                                                         x1(_x1),
	                                                                                                         z0(_z0),
	                                                                                                         z1(_z1),
	                                                                                                         k(_k),
	                                                                                                         mp(mat),
	                                                                                                         flip_normals(
		                                                                                                         flip_normals)
	{
	};


	bool bounding_box(float t0, float t1, AABB& b) const override
	{
		b = AABB({x0, k - 0.0001f, z0}, {x1, k + 0.0001f, z1});
		return true;
	}

	bool hit(const Ray& r, float t_min, float t_max, HitRecord& hit_record) const override
	{
		Ray ray = r;
		HitRecord temp_rec;
		float t = (k - ray.origin.y) / ray.direction.y;

		if (t < t_min || t > t_max)
			return false;

		float x = ray.origin.x + t * ray.direction.x;
		float z = ray.origin.z + t * ray.direction.z;

		if (x < x0 || x > x1 || z < z0 || z > z1)
			return false;


		temp_rec.u = (x - x0) / (x1 - x0);
		temp_rec.v = (z - z0) / (z1 - z0);
		temp_rec.t = t;
		temp_rec.mat_ptr = mp;
		temp_rec.position = ray.point_at_parameter(t);
		temp_rec.normal = flip_normals ? Vector3::UNIT_Y_NEG : Vector3::UNIT_Y_POS;

		hit_record = temp_rec;
		return true;
	}
};


class YZRect : public Hitable
{
	float y0, y1, z0, z1, k;
	Material* mp;
	bool flip_normals;
public:
	YZRect()
	{
	};

	YZRect(float _y0, float _y1, float _z0, float _z1, float _k, Material* mat, bool flip_normals = false) : y0(_y0),
	                                                                                                         y1(_y1),
	                                                                                                         z0(_z0),
	                                                                                                         z1(_z1),
	                                                                                                         k(_k),
	                                                                                                         mp(mat),
	                                                                                                         flip_normals(
		                                                                                                         flip_normals)
	{
	};


	bool bounding_box(float t0, float t1, AABB& b) const override
	{
		b = AABB({k - 0.0001f, y0, z0}, {k + 0.0001f, y1, z1});
		return true;
	}

	bool hit(const Ray& r, float t_min, float t_max, HitRecord& hit_record) const override
	{
		Ray ray = r;
		float t = (k - ray.origin.x) / ray.direction.x;

		if (t < t_min || t > t_max)
			return false;

		float y = ray.origin.y + t * ray.direction.y;
		float z = ray.origin.z + t * ray.direction.z;

		if (y < y0 || y > y1 || z < z0 || z > z1)
			return false;

		HitRecord temp_rec;
		temp_rec.u = (y - y0) / (y1 - y0);
		temp_rec.v = (z - z0) / (z1 - z0);
		temp_rec.t = t;
		temp_rec.mat_ptr = mp;
		temp_rec.position = ray.point_at_parameter(t);
		temp_rec.normal = flip_normals ? Vector3::UNIT_X_NEG : Vector3::UNIT_X_POS;
		hit_record = temp_rec;
		return true;
	}
};
