#pragma once
#include <FFGLSDK.h>

class FFGLGradients : public CFreeFrameGLPlugin
{
public:
	FFGLGradients();

	FFResult SetFloatParameter( unsigned int dwIndex, float value ) override;
	float GetFloatParameter( unsigned int index ) override;
	FFResult ProcessOpenGL( ProcessOpenGLStruct* pGL ) override;
	FFResult InitGL( const FFGLViewportStruct* vp ) override;
	FFResult DeInitGL() override;

protected:
	ffglex::FFGLShader shader;
	ffglex::FFGLScreenQuad quad;
	GLint rgbLeftLocation;
	GLint rgbRightLocation;

	float m_Hue1;
	float m_Hue2;
	float m_Saturation;
	float m_Brightness;
};
