#pragma once
#include <FFGLSDK.h>
class Screenshake : public Effect
{
public:
	Screenshake();
	~Screenshake();
	void update() override;

private:
	float relativeTime = 0;
	ParamRange::Ptr amountX, amountY;
};
