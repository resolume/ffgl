#pragma once
#include <FFGLSDK.h>
class StroboBeat : public Effect
{
public:
	StroboBeat();
	void update() override;
	~StroboBeat();
	Param::Ptr dutyCycle;
	ParamRange::Ptr speed;
};
