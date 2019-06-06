#pragma once
#include "../ffgl/FFGL.h"

namespace ffglex
{
class FFGLFBO
{
public:
	FFGLFBO();

	int Create( int width, int height );
	int Create( int width, int height, GLuint pixelFormat );
	int BindAsRenderTarget();
	int UnbindAsRenderTarget( GLuint hostFbo );

	FFGLTextureStruct GetTextureInfo() const;
	void FreeResources();
	void ResizeViewPort();

	GLuint GetWidth()
	{
		return m_width;
	}
	GLuint GetHeight()
	{
		return m_height;
	}
	GLuint GetFBOHandle() const
	{
		return m_fboHandle;
	}

protected:
	GLuint m_width;
	GLuint m_height;
	GLuint m_glWidth;
	GLuint m_glHeight;
	GLuint m_glPixelFormat;
	GLuint m_glTextureHandle;
	GLuint m_fboHandle;
	GLuint m_depthBufferHandle;
};

}//End namespace ffglex