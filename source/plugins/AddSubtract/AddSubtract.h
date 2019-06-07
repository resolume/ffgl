#pragma once
#include <string>
#include <FFGLSDK.h>

class AddSubtract : public Plugin
{
public:
	AddSubtract();
	~AddSubtract();

	//CFreeFrameGLPlugin
	FFResult InitGL( const FFGLViewportStruct* vp ) override;
	FFResult ProcessOpenGL( ProcessOpenGLStruct* pGL ) override;
	FFResult DeInitGL() override;

private:
	ffglex::FFGLShader shader;   //!< Utility to help us compile and link some shaders into a program.
	ffglex::FFGLScreenQuad quad; //!< Utility to help us render a full screen quad.
	GLint maxUVLocation;
	GLint brightnessLocation;
};
