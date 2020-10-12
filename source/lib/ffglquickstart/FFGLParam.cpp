#include "FFGLParam.h"

namespace ffglqs
{
std::shared_ptr< Param > Param::Create()
{
	return Create( "" );
}
std::shared_ptr< Param > Param::Create( std::string name )
{
	return Create( name, 0 );
}
std::shared_ptr< Param > Param::Create( std::string name, float value )
{
	return Create( name, FF_TYPE_STANDARD, value );
}
std::shared_ptr< Param > Param::Create( std::string name, FFUInt32 type, float value )
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

void Param::SetValue( float v )
{
	value = v;
}

float Param::GetValue() const
{
	return value;
}
const std::string& Param::GetName() const
{
	return name;
}
FFUInt32 Param::GetType() const
{
	return type;
}

}//End namespace ffglqs