#pragma once
#include "Hitable.h"
#include "Vector3.h"
#include "Ray.h"
#include "HitRecord.h"


class Box : public Hitable
{
public:
	Vector3 center;
	Vector3 dimensions;
	Material* mat_ptr;

	Box(const Vector3& center, const Vector3& dimensions, Material* mat)
	{
		this->center = center;
		this->dimensions = dimensions;
		this->mat_ptr = mat;
	}

	void updateHitRecord(const Vector3& position, int u, int v, const Vector3& normal, float t, Material* mat_ptr,
	                     HitRecord& rec) const;

	bool hit(const Ray& ray, float t_min, float t_max, HitRecord& hit_record) const override
	{
		bool hit_anything = false;
		//6 Bounding Box sides
		float BOTTOM = center.y - dimensions.y / 2;
		float TOP = center.y + dimensions.y / 2;
		float BACK = center.z - dimensions.z / 2;
		float FRONT = center.z + dimensions.z / 2;
		float LEFT = center.x - dimensions.x / 2;
		float RIGHT = center.x + dimensions.x / 2;


		//Calculates the position the ray would intersect with each plane
		//If there is an intersection the t value is stored in ts[];
		//When there are two intersection we can early return

		float t;

		//LEFT SIDE
		Vector3 p = ray.point_at_plane_x(LEFT); //Point of intersection
		//If intersection is within the quad save t
		if (p.y > BOTTOM && p.y < TOP && p.z > BACK && p.z < FRONT)
		{
			t = ray.parameter_at_plane_x(LEFT);
			if (!hit_anything || t < hit_record.t)
				if (t > t_min && t < t_max)
				{
					updateHitRecord(p, 1, 2, Vector3::UNIT_X_NEG, t, mat_ptr, hit_record);
					hit_anything = true;
				}
		}

		//RIGHT SIDE
		p = ray.point_at_plane_x(RIGHT);
		if (p.y > BOTTOM && p.y < TOP && p.z > BACK && p.z < FRONT)
		{
			t = ray.parameter_at_plane_x(RIGHT);
			if (!hit_anything || t < hit_record.t)
				if (t > t_min && t < t_max)
				{
					updateHitRecord(p, 1, 2, Vector3::UNIT_X_POS, t, mat_ptr, hit_record);
					hit_anything = true;
				}
		}


		//BOTTOM SIDE
		p = ray.point_at_plane_y(BOTTOM);
		if (p.x > LEFT && p.x < RIGHT && p.z > BACK && p.z < FRONT)
		{
			t = ray.parameter_at_plane_y(BOTTOM);
			if (!hit_anything || t < hit_record.t)
				if (t > t_min && t < t_max)
				{
					updateHitRecord(p, 0, 2, Vector3::UNIT_Y_NEG, t, mat_ptr, hit_record);
					hit_anything = true;
				}
		}

		//TOP SIDE
		p = ray.point_at_plane_y(TOP);
		if (p.x > LEFT && p.x < RIGHT && p.z > BACK && p.z < FRONT)
		{
			t = ray.parameter_at_plane_y(TOP);
			if (!hit_anything || t < hit_record.t)
				if (t > t_min && t < t_max)
				{
					updateHitRecord(p, 0, 2, Vector3::UNIT_Y_POS, t, mat_ptr, hit_record);
					hit_anything = true;
				}
		}

		//BACK SIDE
		p = ray.point_at_plane_z(BACK);
		if (p.x > LEFT && p.x < RIGHT && p.y > BOTTOM && p.y < TOP)
		{
			t = ray.parameter_at_plane_z(BACK);
			if (!hit_anything || t < hit_record.t)
				if (t > t_min && t < t_max)
				{
					updateHitRecord(p, 0, 1, Vector3::UNIT_Z_NEG, t, mat_ptr, hit_record);
					hit_anything = true;
				}
		}

		//FRONT SIDE
		p = ray.point_at_plane_z(FRONT);
		if (p.x > LEFT && p.x < RIGHT && p.y > BOTTOM && p.y < TOP)
		{
			t = ray.parameter_at_plane_z(FRONT);
			if (!hit_anything || t < hit_record.t)
				if (t > t_min && t < t_max)
				{
					updateHitRecord(p, 0, 1, Vector3::UNIT_Z_POS, t, mat_ptr, hit_record);
					hit_anything = true;
				}
		}
		return hit_anything;
	}
};

void Box::updateHitRecord(const Vector3& position, int u, int v, const Vector3& normal, float t, Material* mat_ptr,
                          HitRecord& rec) const
{
	rec.position = position;
	rec.normal = normal;
	rec.t = t;
	rec.mat_ptr = mat_ptr;
	rec.u = ((position[u] - center[u]) / dimensions[u] + 1.0) * 0.5;
	rec.v = ((position[v] - center[v]) / dimensions[v] + 1.0) * 0.5;
}
