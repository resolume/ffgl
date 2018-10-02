#include "FFGLFBO.h"
#include "FFGLScopedTextureBinding.h"

namespace ffglex
{
FFGLFBO::FFGLFBO() :
	m_width( 0 ),
	m_height( 0 ),
	m_glWidth( 0 ),
	m_glHeight( 0 ),
	m_glPixelFormat( 0 ),
	m_glTextureHandle( 0 ),
	m_fboHandle( 0 ),
	m_depthBufferHandle( 0 )
{
}

int FFGLFBO::Create( int _width, int _height )
{
	/*
	int glWidth = 1;
	while (glWidth<_width) glWidth*=2;

	int glHeight = 1;
	while (glHeight<_height) glHeight*=2;
	*/
	m_glWidth = _width;
	m_glHeight = _height;

	m_width = _width;
	m_height = _height;
	m_glWidth = _width;
	m_glHeight = _height;
	m_glPixelFormat = GL_RGBA8;
	m_glTextureHandle = 0;
	glGenFramebuffers( 1, &m_fboHandle );

	return 1;
}

int FFGLFBO::BindAsRenderTarget()
{
	//make our fbo active
	glBindFramebuffer( GL_FRAMEBUFFER, m_fboHandle );

	//make sure there's a valid depth buffer attached to it
	if( glIsRenderbuffer( m_depthBufferHandle ) == 0 )
	{
		glGenRenderbuffers( 1, &m_depthBufferHandle );
		glBindRenderbuffer( GL_RENDERBUFFER, m_depthBufferHandle );
		glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_glWidth, m_glHeight );

		//attach our depth buffer to the fbo
		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferHandle );
	}

	//make sure we have a valid gl texture attached to it
	if( m_glTextureHandle == 0 )
	{
		//get a new one
		glGenTextures( 1, &m_glTextureHandle );

		//bind it for some initialization
		Scoped2DTextureBinding textureBinding( m_glTextureHandle );

		//this only works if the FBO pixel format
		//is GL_RGBA8. other FBO pixel formats have to
		//define their texture differently
		GLuint pformat = GL_RGBA;
		GLuint ptype = GL_UNSIGNED_BYTE;

		glTexImage2D(
			GL_TEXTURE_2D,//texture target
			0,                //mipmap level
			m_glPixelFormat,  //gl internal pixel format
			m_glWidth,        //gl width
			m_glHeight,       //gl height
			0,                //no border
			pformat,          //pixel format #2
			ptype,            //pixel type
			NULL );           //null texture image data pointer

		//for now, do not do mipmapping.
		//to do mipmapping, set this to 1, and after we've bound the texture,
		//we would need to call glGenerateMipmap(m_textureTarget) to make sure
		//the mipmaps have been updated
		int do_mipmaps = 0;

		if( do_mipmaps )
			glGenerateMipmap( GL_TEXTURE_2D );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		if( do_mipmaps )
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		else
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

		textureBinding.EndScope();

		//attach our texture to the FBO
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_glTextureHandle, 0 );
	}

	GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );

	switch( status )
	{
	case GL_FRAMEBUFFER_COMPLETE:
		break; //No Error

	case GL_FRAMEBUFFER_UNDEFINED:
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
	case GL_FRAMEBUFFER_UNSUPPORTED:
		return 0; //Known error

	default: //Unknown error
		return 0;
	}

	return 1;
}

int FFGLFBO::UnbindAsRenderTarget( GLuint hostFbo )
{
	glBindFramebuffer( GL_FRAMEBUFFER, hostFbo );
	return 1;
}

FFGLTextureStruct FFGLFBO::GetTextureInfo()
{
	FFGLTextureStruct t;

	t.Width = m_width;
	t.Height = m_height;

	t.HardwareWidth = m_glWidth;
	t.HardwareHeight = m_glHeight;

	t.Handle = m_glTextureHandle;

	return t;
}

void FFGLFBO::FreeResources()
{
	if( m_fboHandle )
	{
		glDeleteFramebuffers( 1, &m_fboHandle );
		m_fboHandle = 0;
	}

	if( m_depthBufferHandle )
	{
		glDeleteRenderbuffers( 1, &m_depthBufferHandle );
		m_depthBufferHandle = 0;
	}

	if( m_glTextureHandle )
	{
		glDeleteTextures( 1, &m_glTextureHandle );
		m_glTextureHandle = 0;
	}
}

}//End namespace ffglex
