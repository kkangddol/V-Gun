#include "pch.h"
#include "Random.h"
#include <random>

uint32 Random::GenerateRandomNumber(uint32 min, uint32 max)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> distribution(min, max);
	return (uint32)distribution(gen);
}

Vector<uint32> Random::GenerateRandomNumbers(uint32 min, uint32 max, uint32 count)
{
	Vector<uint32> result;

	for (uint32 i = min; i <= max; ++i)
		result.push_back(i);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(result.begin(), result.end(), gen);

	result.resize(count);

	return result;
}
