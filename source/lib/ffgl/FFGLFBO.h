#ifndef FFGLFBO_H
#define FFGLFBO_H

#include <FFGL.h>


class FFGLFBO
{
public:
	FFGLFBO();


	int Create(int width, int height);
	int BindAsRenderTarget();
	int UnbindAsRenderTarget(GLuint hostFbo);

	FFGLTextureStruct GetTextureInfo();
	void FreeResources();

	GLuint GetWidth() { return m_width; }
	GLuint GetHeight() { return m_height; }
	GLuint GetFBOHandle() { return m_fboHandle; }

protected:
	GLuint m_width;
	GLuint m_height;
	GLuint m_glWidth;
	GLuint m_glHeight;
	GLuint m_glPixelFormat;
	GLuint m_glTextureTarget;
	GLuint m_glTextureHandle;
	GLuint m_fboHandle;
	GLuint m_depthBufferHandle;
};

#endif
