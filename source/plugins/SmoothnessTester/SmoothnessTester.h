#pragma once
#include <FFGLSDK.h>

class SmoothnessTester : public CFFGLPlugin
{
public:
	SmoothnessTester();

	//CFFGLPlugin
	FFResult InitGL( const FFGLViewportStruct* vp ) override;
	FFResult ProcessOpenGL( ProcessOpenGLStruct* pGL ) override;
	FFResult DeInitGL() override;

	FFResult SetFloatParameter( unsigned int dwIndex, float value ) override;
	float GetFloatParameter( unsigned int index ) override;

private:
	float m_Width;
	bool m_Compare;

	ffglex::FFGLShader barShader;
	ffglex::FFGLScreenQuad quad;
};
