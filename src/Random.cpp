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
	const std::uniform_int_distribution<int> distribution(0, max);
	const int result = distribution(gen);
	return result;
}

unsigned int Random::randu(unsigned max)
{
	const std::uniform_int_distribution<unsigned> distribution(0, max);
	const unsigned int result = distribution(gen);
	return result;
}

double Random::randd(const double min, const double max)
{
	const std::uniform_real_distribution<double> distribution(min, max);
	const double result = distribution(gen);
	return result;
}

float Random::randf(const float min, const float max)
{
	const std::uniform_real_distribution<float> distribution(min, max);
	const float result = distribution(gen);
	return result;
}

Vector3 Random::random_in_unit_sphere()
{
	Vector3 p;
	do
	{
		p = Vector3(Random::randf(-1, 1), Random::randf(-1, 1), Random::randf(-1, 1));
	}
	while (p.getSquaredLength() >= 1.0);
	return p;
}

std::random_device Random::rd;
std::mt19937 Random::gen;
