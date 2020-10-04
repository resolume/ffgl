#pragma once
#include "FFGLParam.h"

namespace ffglqs
{
class ParamText : public Param
{
public:
	static std::shared_ptr< ParamText > create( std::string name );
	static std::shared_ptr< ParamText > create( std::string name, std::string text );

	ParamText( std::string name );
	ParamText( std::string name, std::string text );

	std::string text;
};

}//End namespace ffglqs