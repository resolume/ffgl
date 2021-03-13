#pragma once
#include <random>

namespace ffglqs
{
class Random
{
public:
	Random();

	int GetRandomInt( int min, int max );
	float GetRandomFloat( float min, float max );

private:
	std::random_device device;
	std::mt19937 rng;
};

}//End namespace ffglqs