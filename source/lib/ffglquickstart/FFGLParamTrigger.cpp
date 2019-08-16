#include "FFGLParamTrigger.h"

namespace ffglqs
{

std::shared_ptr< ParamTrigger > ParamTrigger::create( std::string name )
{
	return std::make_shared< ParamTrigger >( std::move( name ) );
}

ParamTrigger::ParamTrigger( std::string name ) :
	ParamEvent( std::move( name ) )
{
}

void ParamTrigger::consume()
{
	value = false;
}

}//End namespace ffglqs