#pragma once
#include "FFGLParam.h"

namespace ffglqs
{

class ParamRange : public Param
{
public:
	struct Range
	{
		Range( float min, float max );

		float min = 0.0f;
		float max = 1.0f;
	};

	static std::shared_ptr< ParamRange > create( std::string name, float value, Range range );
	static std::shared_ptr< ParamRange > createInteger( std::string name, int value, Range range );

	ParamRange( std::string name, float value, Range range );

	Range getRange() const;
	float getRealValue() const;

protected:
	Range range;
};

}//End namespace ffglqs