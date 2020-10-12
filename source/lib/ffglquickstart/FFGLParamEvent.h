#pragma once
#include "FFGLParam.h"

namespace ffglqs
{
class ParamEvent : public Param
{
public:
	static std::shared_ptr< ParamEvent > Create( std::string name );

	ParamEvent( std::string name );
};

}//End namespace ffglqs