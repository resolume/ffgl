#pragma once
#include <FFGLSDK.h>
class Alea : public Source
{
public:
	Alea();
	void update() override;
	~Alea();

private:
	int sides = 5;
	Param::Ptr smoothness;
	ParamTrigger::Ptr change;
	ParamBool::Ptr fixedSize;
	ParamOption::Ptr select;
	ParamText::Ptr test;
	ParamRange::Ptr gainParam;
};
