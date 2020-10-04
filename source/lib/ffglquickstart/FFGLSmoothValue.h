#pragma once

namespace ffglqs
{
class SmoothValue
{
public:
	SmoothValue() = default;

	void SetSmoothness( float smooth );
	void Update( float val );
	float GetValue();

private:
	float smoothness = 0.80f;
	float value      = 0.0f;
};

}//End namespace ffglqs