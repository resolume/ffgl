#pragma once
#include <FFGLSDK.h>
#include <string>

class Add : public Plugin
{
public:
	Add();
	~Add();

	//CFreeFrameGLPlugin
	FFResult InitGL( const FFGLViewportStruct* vp ) override;
	FFResult ProcessOpenGL( ProcessOpenGLStruct* pGL ) override;
	FFResult DeInitGL() override;

	//The mixer short name
	const char* GetShortName() override
	{
		static const char* sname = "AddB";
		return sname;
	}

private:
	ffglex::FFGLShader shader;   //!< Utility to help us compile and link some shaders into a program.
	ffglex::FFGLScreenQuad quad; //!< Utility to help us render a full screen quad.
	GLint maxUVDestLocation;
	GLint maxUVSrcLocation;
};
