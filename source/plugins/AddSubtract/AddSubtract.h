#pragma once
#include <FFGLSDK.h>
#include <string>

class AddSubtract : public CFreeFrameGLPlugin
{
public:
	AddSubtract();
	~AddSubtract();

	///////////////////////////////////////////////////
	// FreeFrame plugin methods
	///////////////////////////////////////////////////
	FFResult InitGL( const FFGLViewportStruct* vp ) override;
	FFResult DeInitGL() override;

	FFResult ProcessOpenGL( ProcessOpenGLStruct* pGL ) override;

	FFResult SetFloatParameter( unsigned int dwIndex, float value ) override;

	float GetFloatParameter( unsigned int index ) override;

protected:
	// Parameters
	float brightnessR;
	float brightnessG;
	float brightnessB;

	ffglex::FFGLShader shader;
	GLint maxUVLocation;
	GLint brightnessLocation;

	GLuint vaoID;
	GLuint vboID;
};
