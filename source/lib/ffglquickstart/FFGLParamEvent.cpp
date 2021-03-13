#include "FFGLParamEvent.h"

namespace ffglqs
{
std::shared_ptr< ParamEvent > ParamEvent::Create( std::string name )
{
	return std::make_shared< ParamEvent >( std::move( name ) );
}

ParamEvent::ParamEvent( std::string name ) :
	Param( std::move( name ), FF_TYPE_EVENT, 0 )
{
}

}//End namespace ffglqs