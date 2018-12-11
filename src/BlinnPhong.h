#pragma once
#include "Globals.h"
#include "Material.h"
#include "Light.h"
#include "Camera.h"
#include "Hitable.h"
#include <minmax.h>

class BlinnPhong : public Material
{
public:
	Texture* kd;
	Vector3 ks;
	float n;
	bool reflects;
	float fuzz;

	BlinnPhong()
	{
	}

	BlinnPhong(Texture* diffuse, Vector3 specular, float specular_exponent = 0, bool reflects = false, float fuzz = 0.0f)
	{
		kd = diffuse;
		ks = specular;
		n = specular_exponent;
		this->reflects = reflects;
		this->fuzz = fuzz;
	}


	Vector3 emitted(const Ray& ray, const HitRecord& rec) const
	{
		Vector3 diffuse(0);
		Vector3 specular(0);
		HitRecord shadow_rec;

		for (auto&& light : g_lights)
		{
#ifdef  DISTRIBUTED_RAYS
			Vector3 light_position = light.getRandomLightPoint() - rec.position;
#else
			Vector3 light_position = light.center - rec.position;
#endif

			const float distance = light_position.length();
			Vector3 light_dir = light_position.getNormalized();
			const float n_dot_l = rec.normal.dot(light_dir);

			//Offset bias for acne
			Ray ray(rec.position + rec.normal * 0.01f, light_dir, ray.time);

			const bool in_shadow = (world->hit(ray, 0.001f, light_position.length() * 0.999f, shadow_rec));

			diffuse += (1.0f - in_shadow) * n_dot_l * light.color * light.power / distance;
			if (!reflects)
				specular += (1.0f - in_shadow) * getSpecular(camera.position - rec.position,
				                                             light_position, rec.normal,
				                                             light.color, light.power);
		}
		diffuse /= float(g_lights.size());
		specular /= float(g_lights.size());
		diffuse += AMBIENT_LIGHT;
		diffuse.clamp(0, 1);
		specular.clamp(0, 100);

		Vector3 color = diffuse * kd->value(rec.u,rec.v,rec.position) + specular * ks;
		return color;
	}

	bool scatter(const Ray& ray_in, const HitRecord& rec, Vector3& attenuation, Ray& scattered_ray_out) const override
	{
		if(!reflects) return false;
		Vector3 reflected = reflect(ray_in.direction.getNormalized(), rec.normal);
	
		HitRecord shadow_rec;
		Vector3 specular(0);
		for (auto&& light : g_lights)
		{
#ifdef  DISTRIBUTED_RAYS
			Vector3 light_position = light.getRandomLightPoint() - rec.position;
#else
			Vector3 light_position = light.center - rec.position;
#endif
			const float distance = light_position.length();
			Vector3 light_dir = light_position.getNormalized();
			const float n_dot_l = rec.normal.dot(light_dir);

			//Offset bias for acne
			Ray ray(rec.position + rec.normal * 0.01f, light_dir, ray_in.time);

			const bool in_shadow = (world->hit(ray, 0.001f, light_position.length() * 0.999f, shadow_rec));
			specular += (1.0f - in_shadow) * getSpecular(camera.position - rec.position,
			                                             light_position, rec.normal,
			                                             light.color, light.power);
		}
		specular.clamp(0, 100);
		attenuation = ks * specular;
		scattered_ray_out.origin = rec.position;

#ifdef DISTRIBUTED_RAYS
		scattered_ray_out.direction = reflected + fuzz * Random::random_in_unit_sphere();
#else
		scattered_ray_out.direction = reflected;
#endif
		return (reflected.dot(rec.normal) > 0);
	}


	Vector3 getSpecular(const Vector3& view_dir, const Vector3& relative_light_pos, const Vector3& normal,
	                    const Vector3& light_color, const Vector3& light_power) const
	{
		const float distance_to_light = relative_light_pos.length();
		const Vector3 half = (view_dir.getNormalized() + relative_light_pos.getNormalized()).getNormalized();

		const float n_dot_h = normal.dot(half);


		Vector3 specular = ks * light_color * pow(max(0.f, n_dot_h), n) * light_power / distance_to_light;
		return specular;
	}
};
