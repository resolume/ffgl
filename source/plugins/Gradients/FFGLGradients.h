#pragma once
#include <FFGLSDK.h>

class FFGLGradients : public CFreeFrameGLPlugin
{
public:
	FFGLGradients();

	//CFreeFrameGLPlugin
	FFResult InitGL( const FFGLViewportStruct* vp ) override;
	FFResult ProcessOpenGL( ProcessOpenGLStruct* pGL ) override;
	FFResult DeInitGL() override;

	FFResult SetFloatParameter( unsigned int dwIndex, float value ) override;

	float GetFloatParameter( unsigned int index ) override;

private:
	float m_Hue1;
	float m_Hue2;
	float m_Saturation;
	float m_Brightness;

	ffglex::FFGLShader shader;   //!< Utility to help us compile and link some shaders into a program.
	ffglex::FFGLScreenQuad quad; //!< Utility to help us render a full screen quad.
	GLint rgbLeftLocation;
	GLint rgbRightLocation;
};
