#include "FFGLParamOption.h"

namespace ffglqs
{
std::shared_ptr< ParamOption > ParamOption::Create( std::string name, std::vector< Option > options )
{
	return Create( name, std::move( options ), 0 );
}
std::shared_ptr< ParamOption > ParamOption::Create( std::string name, std::vector< Option > options, int defaultOption )
{
	return std::make_shared< ParamOption >( name, std::move( options ), defaultOption );
}

ParamOption::ParamOption( std::string name, std::vector< Option > options ) :
	ParamOption( name, std::move( options ), 0 )
{
}
ParamOption::ParamOption( std::string name, std::vector< Option > options, int defaultOption ) :
	Param( name, FF_TYPE_OPTION, (float)defaultOption ),
	options( std::move( options ) )
{
	SetValue( (float)defaultOption );
}

void ParamOption::SetValue( float newValue )
{
	size_t valueAsIndex = static_cast< size_t >( newValue );

	if( options.size() <= valueAsIndex )
		value = 0;
	else
		value = newValue;

	currentOption = options[ valueAsIndex ];
}

float ParamOption::GetRealValue() const
{
	return currentOption.value;
}

bool ParamOption::IsCurrentOption( const std::string& optionName ) const
{
	return currentOption.name == optionName;
}

}//End namespace ffglqs