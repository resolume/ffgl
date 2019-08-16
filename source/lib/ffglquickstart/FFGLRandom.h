#pragma once
#include <random>

namespace ffglqs
{

class Random
{
public:
	Random();

	int getRandomInt( int min, int max );

	float getRandomFloat( float min, float max );

private:
	std::random_device device;
	std::mt19937 rng;
};

}//End namespace ffglqs