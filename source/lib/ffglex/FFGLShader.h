#pragma once
#include "../ffgl/FFGL.h"//For OpenGL
#include <string>

namespace ffglex
{
class FFGLShader
{
public:
	FFGLShader();
	virtual ~FFGLShader();

	int IsReady();

	int Compile( const char* vtxProgram, const char* fragProgram );
	int Compile( const std::string& vtxProgram, const std::string& fragProgram );

	GLuint FindUniform( const char* name );
	void FreeGLResources();

	GLuint GetGLID() const;

private:
	void CreateGLResources();

	GLuint m_glProgram;
	GLenum m_glVertexShader;
	GLenum m_glFragmentShader;
	GLuint m_linkStatus;
};

}//End namespace ffglex