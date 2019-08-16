#include "FFGLSmoothValue.h"

namespace ffglqs
{

void SmoothValue::setSmoothness( float smooth )
{
	smoothness = smooth;
}
void SmoothValue::update( float val )
{
	value *= smoothness;
	value += ( 1 - smoothness ) * val;
}
float SmoothValue::getValue()
{
	return value;
}

}//End namespace ffglqs