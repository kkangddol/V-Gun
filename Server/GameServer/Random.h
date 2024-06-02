#pragma once


class Random
{
public:
	static uint32 GenerateRandomNumber(uint32 min, uint32 max);
	static Vector<uint32> GenerateRandomNumbers(uint32 min, uint32 max, uint32 count);
};

