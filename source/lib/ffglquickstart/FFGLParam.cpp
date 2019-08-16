#include "FFGLParam.h"

namespace ffglqs
{

std::shared_ptr< Param > Param::create()
{
	return create( "" );
}
std::shared_ptr< Param > Param::create( std::string name )
{
	return create( name, 0 );
}
std::shared_ptr< Param > Param::create( std::string name, float value )
{
	return create( name, FF_TYPE_STANDARD, value );
}
std::shared_ptr< Param > Param::create( std::string name, FFUInt32 type, float value )
{
	return std::make_shared< Param >( name, type, value );
}

Param::Param() :
	Param( "" )
{
}
Param::Param( std::string name ) :
	Param( std::move( name ), 0 )
{
}
Param::Param( std::string name, float value ) :
	Param( std::move( name ), FF_TYPE_STANDARD, value )
{
}
Param::Param( std::string name, FFUInt32 type, float value ) :
	name( std::move( name ) ),
	type( type ),
	value( value )
{
}

void Param::setValue( float v )
{
	value = v;
}

float Param::getValue() const
{
	return value;
}
const std::string& Param::getName() const
{
	return name;
}
FFUInt32 Param::getType() const
{
	return type;
}

}//End namespace ffglqs