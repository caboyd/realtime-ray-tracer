#pragma once

#include "HitRecord.h"
#include "Vector3.h"
#include "Random.h"
#include "Ray.h"

Vector3 reflect(const Vector3& v, const Vector3& n);

class Material
{
public:
	virtual ~Material() = default;
	virtual bool scatter(const Ray& ray_in, const HitRecord& rec, Vector3& attenuation,
	                     Ray& scattered_ray_out) const = 0;
};

class Lambertian : public Material
{
public:
	Vector3 albedo;

	Lambertian(const Vector3& a) : albedo(a)
	{
	}

	bool scatter(const Ray& ray_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered_ray_out) const override
	{
		const Vector3 out_direction = rec.normal + Random::random_in_unit_sphere();
		scattered_ray_out = Ray(rec.position, out_direction);
		attenuation = albedo;
		return true;
	}
};


class Metal : public Material
{
public:
	Vector3 albedo;

	Metal(const Vector3& a) : albedo(a)
	{
	}

	bool scatter(const Ray& ray_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered_ray_out) const override
	{
		Vector3 reflected = reflect(ray_in.direction.getNormalized(), rec.normal);
		scattered_ray_out = Ray(rec.position, reflected);
		attenuation = albedo;
		return (scattered_ray_out.direction.dot(rec.normal) > 0);
	}
};

//Reflect Equation
Vector3 reflect(const Vector3& v, const Vector3& n)
{
	return v - 2 * v.dot(n) * n;
}
