#pragma once
#include <FFGLSDK.h>
using namespace ffglex;

class Bloom : public Effect
{
public:
	Bloom();
	FFResult init() override;
	FFResult render( ProcessOpenGLStruct* inputTextures ) override;
	void clean() override;
	~Bloom();

private:
	ParamRange::Ptr threshold;
	ParamRange::Ptr radius;
	ParamRange::Ptr intensity;
	ParamBool::Ptr hq;
	ParamBool::Ptr antiFlicker;
	ParamRange::Ptr jitter;
	FFGLShader downSampleFilter;
	FFGLShader upSampleFilter;
	FFGLShader final;
};
