#pragma once
#include "Material.h"
#include "Globals.h"

class Metal : public Material
{
public:
	Vector3 albedo;
	float fuzz;

	Metal(const Vector3& a, float f) : albedo(a)
	{
		if (f < 1) fuzz = f;
		else fuzz = 1;
	}

	bool scatter(const Ray& ray_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered_ray_out) const override
	{
		scattered_ray_out.time = ray_in.time;
		Vector3 reflected = reflect(ray_in.direction.getNormalized(), rec.normal);
		attenuation = albedo;
		scattered_ray_out.origin = rec.position;

#ifdef DISTRIBUTED_RAYS
		scattered_ray_out.direction = reflected + fuzz * Random::random_in_unit_sphere();
#else
		scattered_ray_out.direction = reflected;
#endif

		scattered_ray_out.origin = scattered_ray_out.direction.dot(rec.normal) < 0
			                           ? scattered_ray_out.origin - rec.normal * 0.001f
			                           : scattered_ray_out.origin + rec.normal * 0.001f;


		return (scattered_ray_out.direction.dot(rec.normal) > 0);
	}

};
