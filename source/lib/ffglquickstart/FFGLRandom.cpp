#include "FFGLRandom.h"

namespace ffglqs
{
Random::Random() :
	rng( std::mt19937( device() ) )
{
}

int Random::GetRandomInt( int min, int max )
{
	std::uniform_int_distribution<> distribution( min, max );
	return distribution( rng );
}
float Random::GetRandomFloat( float min, float max )
{
	std::uniform_real_distribution<> distribution( min, max );
	return (float)distribution( rng );
}

}//End namespace ffglqs