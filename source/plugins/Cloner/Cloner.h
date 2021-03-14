#pragma once
#include <string>
#include <ffgl/FFGLPluginSDK.h>
#include <ffglex/FFGLShader.h>
#include <ffglex/FFGLScreenQuad.h>

class Cloner : public CFFGLPlugin
{
public:
	Cloner();
	~Cloner();

	//CFreeFrameGLPlugin
	FFResult InitGL( const FFGLViewportStruct* vp ) override;
	FFResult ProcessOpenGL( ProcessOpenGLStruct* pGL ) override;
	FFResult DeInitGL() override;

	FFResult SetFloatParameter( unsigned int dwIndex, float value ) override;

	float GetFloatParameter( unsigned int index ) override;

private:
	float numClones;
	float scale;
	float radius;
	float fan;

	ffglex::FFGLShader shader;  //!< Utility to help us compile and link some shaders into a program.
	ffglex::FFGLScreenQuad quad;//!< Utility to help us render a full screen quad.
	GLint maxUVLocation;
	GLint aspectLocation;
	GLint numClonesLocation;
	GLint scaleLocation;
	GLint radiusLocation;
	GLint fanLocation;
};
