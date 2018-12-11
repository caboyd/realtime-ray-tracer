#pragma once

#include "HitRecord.h"
#include "Vector3.h"
#include "Random.h"
#include "Ray.h"
#include "Texture.h"
#include "Globals.h"

inline Vector3 reflect(const Vector3& v, const Vector3& n);
inline bool refract(const Vector3& v, const Vector3& n, float ni_over_nt, Vector3& refracted);
inline float schlick(float cosine, float ref_idx);

class Material
{
public:
	virtual ~Material() = default;
	virtual bool scatter(const Ray& ray_in, const HitRecord& rec, Vector3& attenuation,
	                     Ray& scattered_ray_out) const = 0;
	virtual bool scatterTwo(const Ray& ray_in, const HitRecord& rec, Vector3& attenuation,
	                     Ray& scattered_ray_out) const { return false; }

	virtual Vector3 emitted(const Ray& ray, const HitRecord& rec) const { return Vector3::ZERO; }

	virtual bool reflection(const Ray& ray_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered_ray_out) const
	{
		return false;
	}
};

class Lambertian : public Material
{
public:
	Texture* albedo;

	Lambertian(Texture* a) : albedo(a)
	{
	}

	bool scatter(const Ray& ray_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered_ray_out) const override
	{
		const Vector3 out_direction = rec.normal + Random::random_in_unit_sphere();	
		scattered_ray_out.time = ray_in.time;
		scattered_ray_out.origin = rec.position;
		scattered_ray_out.direction = out_direction;
		attenuation = albedo->value(rec.u,rec.v, rec.position);
		return true;
	}
};



class Dialectric : public Material
{
public:
	float ref_idx;
	float blur;
	Vector3 albedo;

	Dialectric(const Vector3& a, float ri, float blur = 0.f) : albedo(a), ref_idx(ri), blur(blur)
	{
	}

	bool reflection(const Ray& ray_in, const HitRecord& rec, Vector3& attenuation,
	                Ray& scattered_ray_out) const override
	{
		scattered_ray_out.time = ray_in.time;
		Vector3 reflected = reflect(ray_in.direction.getNormalized(), rec.normal);

		Vector3 outward_normal;
		float ni_over_nt;
		Vector3 refracted;
		float reflect_prob = 1.f;
		float cosine;

		if (ray_in.direction.dot(rec.normal) > 0.0f)
		{
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * ray_in.direction.dot(rec.normal) / ray_in.direction.length();
		}
		else
		{
			outward_normal = rec.normal;
			ni_over_nt = 1.0f / ref_idx;
			cosine = -ray_in.direction.dot(rec.normal) / ray_in.direction.length();
		}

		if (refract(ray_in.direction, outward_normal, ni_over_nt, refracted))
			reflect_prob = schlick(cosine, ref_idx);

		attenuation = Vector3(reflect_prob * albedo);
		scattered_ray_out.origin = rec.position;
		scattered_ray_out.direction = reflected;
		scattered_ray_out.origin = scattered_ray_out.direction.dot(rec.normal) < 0
			                           ? scattered_ray_out.origin - rec.normal * 0.01f
			                           : scattered_ray_out.origin + rec.normal * 0.01f;

		return (scattered_ray_out.direction.dot(rec.normal) > 0);
	}

	bool scatter(const Ray& ray_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered_ray_out) const override
	{
		Vector3 outward_normal;
		Vector3 reflected = reflect(ray_in.direction, rec.normal);
		float ni_over_nt;
		attenuation = Vector3::ONE;
		Vector3 refracted;
		float reflect_prob;
		float cosine;

		if (ray_in.direction.dot(rec.normal) > 0.0f)
		{
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * ray_in.direction.dot(rec.normal) / ray_in.direction.length();
		}
		else
		{
			outward_normal = rec.normal;
			ni_over_nt = 1.0f / ref_idx;
			cosine = -ray_in.direction.dot(rec.normal) / ray_in.direction.length();
		}

		if (refract(ray_in.direction, outward_normal, ni_over_nt, refracted))
			reflect_prob = schlick(cosine, ref_idx);
		else
			reflect_prob = 1.0;

		scattered_ray_out.origin = rec.position;

		if (Random::randf(0, 1) < reflect_prob)
		{
			attenuation = albedo;
			scattered_ray_out.direction = reflected;
		}
		else
			scattered_ray_out.direction = refracted + blur * Random::random_in_unit_sphere();


		return true;
	}
};


class DiffuseLight : public Material
{
public:
	Texture* emit;

	DiffuseLight(Texture* a) : emit(a)
	{
	}

	bool scatter(const Ray& ray_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered_ray_out) const override
	{
		return false;
	}

	Vector3 emitted(const Ray& ray, const HitRecord& rec) const override
	{
		return emit->value(rec.u, rec.v, rec.position);
	}
};

//Reflect Equation
Vector3 reflect(const Vector3& v, const Vector3& n)
{
	return v - 2 * v.dot(n) * n;
}

bool refract(const Vector3& v, const Vector3& n, float ni_over_nt, Vector3& refracted)
{
	Vector3 uv = v.getNormalized();
	float dt = uv.dot(n);
	float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1.0f - dt * dt);
	if (discriminant > 0.0f)
	{
		refracted = ni_over_nt * (uv - n * dt) - n * sqrtf(discriminant);

		//OpenGL Formula - v,n must be normalized
		//refracted = ni_over_nt * uv - (ni_over_nt * dt + sqrtf(discriminant)) * n.getNormalized();
		return true;
	}
	return false;
}

float schlick(float cosine, float ref_idx)
{
	float r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
	r0 = r0 * r0;
	return r0 + (1.0f - r0) * pow((1.0f - cosine), 5.0f);
}
