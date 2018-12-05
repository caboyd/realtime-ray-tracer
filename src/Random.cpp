#pragma once
#include "Random.h"
#include "Vector3.h"

Random::Random()
{
	gen.seed(rd());
}

void Random::init()
{
	gen.seed(rd());
}

int Random::randi(int max)
{
	return randi(gen, max);
}

int Random::randi(std::mt19937& gen, int max)
{
	const std::uniform_int_distribution<int> distribution(0, max);
	const int result = distribution(gen);
	return result;
}

unsigned int Random::randu(unsigned max)
{
	return randu(gen, max);
}

unsigned Random::randu(std::mt19937& gen, unsigned max)
{
	const std::uniform_int_distribution<unsigned> distribution(0, max);
	const unsigned int result = distribution(gen);
	return result;
}


double Random::randd(const double min, const double max)
{
	return randd(gen, min, max);
}

double Random::randd(std::mt19937& gen, double min, double max)
{
	const std::uniform_real_distribution<double> distribution(min, max);
	const double result = distribution(gen);
	return result;
}


float Random::randf(const float min, const float max)
{
	return randf(gen, min, max);
}

float Random::randf(std::mt19937& gen, float min, float max)
{
	const std::uniform_real_distribution<float> distribution(min, max);
	const float result = distribution(gen);
	return result;
}

Vector3 Random::random_in_unit_sphere()
{
	return random_in_unit_sphere(gen);
}

Vector3 Random::random_in_unit_sphere(std::mt19937& gen)
{
	Vector3 p;
	do
	{
		p.set(Random::randf(gen, -1, 1), Random::randf(gen, -1, 1), Random::randf(gen, -1, 1));
	}
	while (p.getSquaredLength() >= 1.0);
	return p;
}

Vector3 Random::random_in_unit_disk()
{
	return random_in_unit_disk(gen);
}

Vector3 Random::random_in_unit_disk(std::mt19937& gen)
{
	Vector3 p;
	do
	{
		p.set(Random::randf(gen, -1, 1), Random::randf(gen, -1, 1), 0);
	}
	while (p.getSquaredLength() >= 1.0);
	return p;
}

std::random_device Random::rd;
std::mt19937 Random::gen;
