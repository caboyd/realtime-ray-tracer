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
	static int randi(std::mt19937& gen, int max);
	//Get random uint from [0, max)
	static unsigned int randu(unsigned int max);
	static unsigned int randu(std::mt19937& gen, unsigned int max);
	//Get random double from [min, max]
	static double randd(double min, double max);
	static double randd(std::mt19937& gen, double min, double max);
	//Get random float from [min, max]
	static float randf(float min, float max);
	static float randf(std::mt19937& gen, float min, float max);
	static Vector3 random_in_unit_sphere();
	static Vector3 random_in_unit_sphere(std::mt19937& gen);

		static Vector3 random_in_unit_disk();
	static Vector3 random_in_unit_disk(std::mt19937& gen);
};
