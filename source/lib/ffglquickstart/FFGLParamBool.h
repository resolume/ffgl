#pragma once
#include "FFGLParam.h"

namespace ffglqs
{
class ParamBool : public Param
{
public:
	static std::shared_ptr< ParamBool > Create( std::string name );
	static std::shared_ptr< ParamBool > Create( std::string name, bool defaultValue );

	ParamBool( std::string name );
	ParamBool( std::string name, bool defaultValue );
};

}//End namespace ffglqs