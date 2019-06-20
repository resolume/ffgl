#pragma once
#include <FFGLSDK.h>

class FFGLGradients : public Plugin
{
public:
	FFGLGradients();

	//CFreeFrameGLPlugin
	FFResult InitGL( const FFGLViewportStruct* vp ) override;
	FFResult ProcessOpenGL( ProcessOpenGLStruct* pGL ) override;
	FFResult DeInitGL() override;

private:
	Param::Ptr hue1;
	Param::Ptr hue2;
	Param::Ptr saturation;
	Param::Ptr brightness;
	ParamFFT::Ptr fft;

	ffglex::FFGLShader shader;   //!< Utility to help us compile and link some shaders into a program.
	ffglex::FFGLScreenQuad quad; //!< Utility to help us render a full screen quad.
};
