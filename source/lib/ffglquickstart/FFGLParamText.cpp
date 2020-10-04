#include "FFGLParamText.h"

namespace ffglqs
{
std::shared_ptr< ParamText > ParamText::create( std::string name )
{
	return create( std::move( name ), "" );
}
std::shared_ptr< ParamText > ParamText::create( std::string name, std::string text )
{
	return std::make_shared< ParamText >( std::move( name ), text );
}

ParamText::ParamText( std::string name ) :
	ParamText( std::move( name ), "" )
{
}
ParamText::ParamText( std::string name, std::string text ) :
	Param( std::move( name ), FF_TYPE_TEXT, 0 ),
	text( text )
{
}

}//End namespace ffglqs