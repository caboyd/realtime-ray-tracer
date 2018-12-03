#pragma once
#include "Vector3.h"
#include "Ray.h"

class AABB
{
private:
	Vector3 _min;
	Vector3 _max;
public :


	AABB()
	{
	};

	AABB(const Vector3& a, const Vector3& b)
	{
		_min = a;
		_max = b;
	}

	Vector3 min() const { return _min; }
	Vector3 max() const { return _max; };

	bool hit(const Ray& r, float tmin, float tmax) const
	{
		//Method 1
		/*
		for (int a = 0; a < 3; a++)
		{
			float t0 = fmin((_min[a] - r.origin[a]) / r.direction[a],
			                (_max[a] - r.origin[a]) / r.direction[a]);
			float t1 = fmax((_min[a] - r.origin[a]) / r.direction[a],
			                (_max[a] - r.origin[a]) / r.direction[a]);
			tmin = fmax(t0, tmin);
			tmax = fmin(t1, tmax);
			if (tmax <= tmin)
				return false;
		}
		return true;
		*/

		//Method 2
		for (int a = 0; a < 3; a++)
		{
			float invD = 1.0f / r.direction[a];
			float t0 = (_min[a] - r.origin[a]) * invD;
			float t1 = (_max[a] - r.origin[a]) * invD;
			if (invD < 0.0f)
				std::swap(t0, t1);
			tmin = t0 > tmin ? t0 : tmin;
			tmax = t1 < tmax ? t1 : tmax;
			if (tmax <= tmin)
				return false;
		}
		return true;
	}
};
