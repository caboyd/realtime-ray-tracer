#pragma once
#include "Random.h"
#include "Vector3.h"

#define constapm 16807
#define constmpm 2147483647.0

void Random::seed(long unsigned int seed)
{
	g_seed = seed;
}


unsigned long Random::rand31pm_next(unsigned long* seedp)
{
	double const a = constapm;
	double const m = constmpm;
	/* This is the linear congrentual 
	 * generator:
	 *  
	 * Multiply the old seed by constant a 
	 * and take the modulus of the result 
	 * (the remainder of a division) by 
	 * constant m.
	 */

	return (*seedp = long(fmod((*seedp * a), m)));
}

unsigned long Random::rand31pm_next()
{
	return rand31pm_next(&g_seed);
}

int Random::randi(int max)
{
	return randi(&g_seed, max);
}

int Random::randi(unsigned long* seedp, int max)
{
	return (rand31pm_next(seedp) % max);
}


unsigned int Random::randu(unsigned max)
{
	return randu(&g_seed, max);
}

unsigned Random::randu(unsigned long* seedp, unsigned max)
{
	return (rand31pm_next(seedp) % max);
}


double Random::randd(const double min, const double max)
{
	return randd(&g_seed, min, max);
}

double Random::randd(unsigned long* seedp, double min, double max)
{
	return min + (rand31pm_next(seedp) / (2147483647.0/(max - min)) );
}


float Random::randf(const float min, const float max)
{
	return randf(&g_seed, min, max);
}

float Random::randf(unsigned long* seedp, float min, float max)
{
	return float(min + (rand31pm_next(seedp) / (2147483647.0/(max - min))));
}

Vector3 Random::random_in_unit_sphere()
{
	return random_in_unit_sphere(&g_seed);
}

Vector3 Random::random_in_unit_sphere(unsigned long* seedp)
{
	Vector3 p;
	do
	{
		p.set(randf(seedp, -1, 1), randf(seedp, -1, 1), randf(seedp, -1, 1));
	}
	while (p.getSquaredLength() >= 1.0);
	return p;
}

Vector3 Random::random_in_unit_disk()
{
	return random_in_unit_disk(&g_seed);
}

Vector3 Random::random_in_unit_disk(unsigned long* seedp)
{
	Vector3 p;
	do
	{
		p.set(randf(seedp, -1, 1), randf(seedp, -1, 1), 0);
	}
	while (p.getSquaredLength() >= 1.0);
	return p;
}


long unsigned int Random::g_seed = 1;
