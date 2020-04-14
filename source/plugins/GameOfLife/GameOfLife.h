#pragma once
#include <string>
#include <FFGLSDK.h>

class GameOfLife : public ffglqs::Effect
{
public:
	GameOfLife();
	~GameOfLife();

	//CFFGLPlugin
	FFResult InitGL( const FFGLViewportStruct* vp ) override;
	FFResult ProcessOpenGL( ProcessOpenGLStruct* pGL ) override;
	FFResult DeInitGL() override;

private:
	ffglex::FFGLShader shader;   //!< Utility to help us compile and link some shaders into a program.
	ffglex::FFGLScreenQuad quad; //!< Utility to help us render a full screen quad.
	
	std::shared_ptr<ffglqs::ParamEvent> sample;
	GLuint handle;
	FFGLTextureStruct* texture;
};
