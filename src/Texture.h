#pragma once
#include "Vector3.h"

class Texture
{
public:
	virtual Vector3 value(float u, float v, const Vector3& p) const = 0;
};

class ConstantTexture : public Texture
{
public:
	Vector3 color;

	ConstantTexture()
	{
	};

	ConstantTexture(Vector3 c) : color(c)
	{
	}

	Vector3 value(float u, float v, const Vector3& p) const override
	{
		return color;
	}
};


class CheckerTexture : public Texture
{
public :
	Texture* odd;
	Texture* even;
	float step_size;
	CheckerTexture()
	{
	};

	CheckerTexture(Texture* t0, Texture* t1, float step_size): even(t0), odd(t1)
	{
		this->step_size = step_size;
	}

	Vector3 value(float u, float v, const Vector3& p) const override
	{
		float sines = sin(step_size * p.x) * sin(step_size * p.y) * sin(step_size * p.z);
		if (sines < 0.f)
			return odd->value(u, v, p);
		else
			return even->value(u, v, p);
	}
};
