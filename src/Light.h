#pragma once
#include "Vector3.h"
#include "Random.h"

class Light
{
public:
	Vector3 center;
	Vector3 dimensions;
	Vector3 color;
	Vector3 power;

	Light() = default;;

	Light(const Vector3& center, const Vector3& dimensions, const Vector3& color, const Vector3& power)
	{
		this->center = center;
		this->dimensions = dimensions;
		this->color = color;
		this->power = power;
	}

	Vector3 getRandomLightPoint() const
	{
		return Vector3(center.x + Random::randf(-dimensions.x/2, dimensions.x/2),
		               center.y + Random::randf(-dimensions.y/2, dimensions.y/2),
		               center.z + Random::randf(-dimensions.z/2, dimensions.z/2));
	}
};
