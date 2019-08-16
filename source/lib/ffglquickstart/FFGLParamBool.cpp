#include "FFGLParamBool.h"

namespace ffglqs
{

std::shared_ptr< ParamBool > ParamBool::create( std::string name )
{
	return create( std::move( name ), false );
}
std::shared_ptr< ParamBool > ParamBool::create( std::string name, bool defaultValue )
{
	return std::make_shared< ParamBool >( std::move( name ), defaultValue );
}

ParamBool::ParamBool( std::string name ) :
	ParamBool( std::move( name ), false )
{
}
ParamBool::ParamBool( std::string name, bool defaultValue ) :
	Param( std::move( name ), FF_TYPE_BOOLEAN, defaultValue )
{
}

}//End namespace ffglqs