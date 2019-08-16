#pragma once

namespace ffglqs
{

class SmoothValue
{
public:
	SmoothValue() = default;

	void setSmoothness( float smooth );
	void update( float val );
	float getValue();

private:
	float smoothness = 0.80f;
	float value      = 0.0f;
};

}//End namespace ffglqs