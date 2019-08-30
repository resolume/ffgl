#pragma once
#include "../ffgl/FFGL.h"

namespace ffglex
{

class FFGLFBO
{
public:
	FFGLFBO();

	bool Initialise( GLsizei width, GLsizei height, GLint internalColorFormat = GL_RGBA8 );
	void Release();

	void ResizeViewPort();

	GLuint GetWidth() const;
	GLuint GetHeight() const;
	GLuint GetGLID() const;
	FFGLTextureStruct GetTextureInfo() const;

protected:
	bool GenerateFBO();
	bool GenerateDepthBuffer();
	bool GenerateColorTexture();

	GLsizei width;
	GLsizei height;
	GLint internalColorFormat;

	GLuint fboID;
	GLuint depthBufferID;
	GLuint colorTextureID;
};

}//End namespace ffglex