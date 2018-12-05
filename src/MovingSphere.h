#pragma once
#include "Hitable.h"
#include "Sphere.h"

class MovingSphere : public Hitable
{
public:
	Vector3 center0, center1;
	float time0, time1;
	float radius;
	Material* mat_ptr;

	MovingSphere()
	{
	};

	MovingSphere(Vector3 cen0, Vector3 cen1, float t0, float t1, float r, Material* mat) :
		center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(mat)
	{
	};

	Vector3 getCenter(float time) const
	{
		return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
	}

	bool hit(const Ray& ray, float t_min, float t_max, HitRecord& hit_record) const override
	{
		Vector3 center = getCenter(ray.time);

		Vector3 oc = ray.origin - center;

		float a = ray.direction.dot(ray.direction);
		float b = ray.direction.dot(oc);
		float c = oc.dot(oc) - radius * radius;

		//Use quadratic formula
		// (-b +- sqrt(b^2 - 4ac)) / 2a

		//find discriminant first to determine type of roots
		// discriminant = (b^2 - 4ac)
		float discriminant = b * b - a * c;

		if (discriminant < 0)
		{
			//No Roots	
		}
		else if (discriminant >= 0)
		{
			//One or Two Roots
			float t0 = (-b - sqrt(discriminant)) / a;
			if (t0 > t_min && t0 < t_max)
			{
				hit_record.t = t0;
				hit_record.position = ray.point_at_parameter(t0);
				hit_record.normal = (hit_record.position - center) / radius;
				hit_record.mat_ptr = mat_ptr;
				float u, v;
				getSphereUV(hit_record.position - center, u, v);
				hit_record.u = u;
				hit_record.v = v;
				return true;
			}

			float t1 = (-b + sqrt(discriminant)) * a;
			if (t1 > t_min && t1 < t_max)
			{
				hit_record.t = t1;
				hit_record.position = ray.point_at_parameter(t1);
				hit_record.normal = (hit_record.position - center) / radius;
				hit_record.mat_ptr = mat_ptr;
				float u, v;
				getSphereUV(hit_record.position - center, u, v);
				hit_record.u = u;
				hit_record.v = v;
				return true;
			}
		}

		return false;
	}

	bool bounding_box(float t0, float t1, AABB& b) const override
	{
		AABB box0 = AABB(getCenter(t0) - Vector3(radius), getCenter(t0) + Vector3(radius));
		AABB box1 = AABB(getCenter(t1) - Vector3(radius), getCenter(t1) + Vector3(radius));
		b = surrounding_box(box0,box1);
		return true;
	}
};
