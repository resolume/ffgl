#pragma once
#include <FFGLSDK.h>
class Dissolve : public Effect
{
public:
	Dissolve();
	void update() override;
	~Dissolve();

private:
	float relativeTime = 0;
};
