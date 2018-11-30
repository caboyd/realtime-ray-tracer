#pragma once
#include <random>

class Vector3;

class Random
{
private:
    static std::random_device rd;
    static std::mt19937 gen;
public:

	Random();

	static void init();

	//Get random int from [0, max)
	static int randi(int max);

	//Get random uint from [0, max)
	static unsigned int randu(unsigned int max);

	//Get random double from [min, max]
	static double randd(double min, double max);

	//Get random float from [min, max]
	static float randf(float min, float max);

	static Vector3 random_in_unit_sphere();

};

