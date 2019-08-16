#include "FFGLParamRange.h"
#include "../ffglex/FFGLUtilities.h"

namespace ffglqs
{

ParamRange::Range::Range( float min, float max ) :
	min( min ),
	max( max )
{
}

std::shared_ptr< ParamRange > ParamRange::create( std::string name, float value, Range range )
{
	return std::make_shared< ParamRange >( name, value, range );
}
std::shared_ptr< ParamRange > ParamRange::createInteger( std::string name, int value, Range range )
{
	auto res  = std::make_shared< ParamRange >( name, (float)value, range );
	res->type = FF_TYPE_INTEGER;
	return res;
}

ParamRange::ParamRange( std::string name, float value, Range range ) :
	Param( std::move( name ), FF_TYPE_STANDARD ),
	range( range )
{
	setValue( ffglex::map( value, range.min, range.max, 0.0, 1.0 ) );
}

ParamRange::Range ParamRange::getRange() const
{
	return range;
}
float ParamRange::getRealValue() const
{
	return ffglex::map( value, 0.0, 1.0, range.min, range.max );
}

}//End namespace ffglqs