#pragma once
#include <random>

class Vector3;


struct Random
{
	static long unsigned int g_seed;

	static void seed(long unsigned int seed);

	static long unsigned int rand31pm_next(long unsigned int* seedp);
	static long unsigned int rand31pm_next();

	//Get random int from [0, max)
	static int randi(int max);
	static int randi(long unsigned int* seedp, int max);
	//Get random uint from [0, max)
	static unsigned int randu(unsigned int max);
	static unsigned int randu(long unsigned int* seedp, unsigned int max);
	//Get random double from [min, max]
	static double randd(double min, double max);
	static double randd(long unsigned int* seedp, double min, double max);
	//Get random float from [min, max]
	static float randf(float min, float max);
	static float randf(long unsigned int* seedp, float min, float max);

	static Vector3 random_in_unit_sphere();
	static Vector3 random_in_unit_sphere(long unsigned int* seedp);

	static Vector3 random_in_unit_disk();
	static Vector3 random_in_unit_disk(long unsigned int* seedp);
};
