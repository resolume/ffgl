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

	static std::shared_ptr< ParamRange > Create( std::string name, float value, Range range );
	static std::shared_ptr< ParamRange > CreateInteger( std::string name, int value, Range range );

	ParamRange( std::string name, float value, Range range );

	Range GetRange() const;
	float GetNormalizedValue() const;

protected:
	Range range;
};

}//End namespace ffglqs