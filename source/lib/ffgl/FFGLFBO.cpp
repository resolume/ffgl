#include "FFGLFBO.h"

FFGLFBO::FFGLFBO():
	m_width(0),
	m_height(0),
	m_glWidth(0),
	m_glHeight(0),
	m_glPixelFormat(0),
	m_glTextureTarget(0),
	m_glTextureHandle(0),
	m_fboHandle(0),
	m_depthBufferHandle(0)
{

}

int FFGLFBO::Create(int _width, int _height)
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
	m_glTextureTarget = GL_TEXTURE_2D;
	m_glTextureHandle = 0;
	glGenFramebuffers(1, &m_fboHandle);

	return 1;
}

int IsTextureResident(GLuint handle)
{
	GLboolean b;
	if (glAreTexturesResident(1, &handle, &b))  
		return 1;
	return 0;
}


int FFGLFBO::BindAsRenderTarget()
{
	//make our fbo active
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboHandle);

	//make sure there's a valid depth buffer attached to it
	if (glIsRenderbufferEXT(m_depthBufferHandle)==0)
	{
		glGenRenderbuffersEXT(1, &m_depthBufferHandle);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depthBufferHandle);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, m_glWidth, m_glHeight);

		//attach our depth buffer to the fbo
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER_EXT, m_depthBufferHandle);
	}

	//make sure we have a valid gl texture attached to it
	if (!IsTextureResident(m_glTextureHandle))
	{
		//get a new one
		glGenTextures(1,&m_glTextureHandle);

		//bind it for some initialization
		glBindTexture(m_glTextureTarget, m_glTextureHandle);

		//this only works if the FBO pixel format
		//is GL_RGBA8. other FBO pixel formats have to
		//define their texture differently
		GLuint pformat = GL_RGBA;
		GLuint ptype = GL_UNSIGNED_BYTE;

		glTexImage2D(
			m_glTextureTarget,	//texture target
			0,					//mipmap level
			m_glPixelFormat,	//gl internal pixel format
			m_glWidth,			//gl width
			m_glHeight,			//gl height
			0,					//no border
			pformat,			//pixel format #2
			ptype,				//pixel type
			NULL);				//null texture image data pointer

		//for now, do not do mipmapping.
		//to do mipmapping, set this to 1, and after we've bound the texture,
		//we would need to call glGenerateMipmap(m_textureTarget) to make sure
		//the mipmaps have been updated
		int do_mipmaps = 0;

		if (do_mipmaps)
			glGenerateMipmap(m_glTextureTarget);

		glTexParameteri(m_glTextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (do_mipmaps)
			glTexParameteri(m_glTextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glTexParameteri(m_glTextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(m_glTextureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_glTextureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//unbind the texture
		glBindTexture(m_glTextureTarget, 0);

		//attach our texture to the FBO
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_glTextureTarget, m_glTextureHandle, 0);
	}  

	GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );

	switch(status)
	{
		case GL_FRAMEBUFFER_COMPLETE:
			//no error
			break;

		case GL_FRAMEBUFFER_UNSUPPORTED:
			//FFDebugMessage("GL_FRAMEBUFFER_UNSUPPORTED");
			return 0;

		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			//FFDebugMessage("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
			return 0;

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			//FFDebugMessage("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
			return 0;

		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			//FFDebugMessage("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT");
			return 0;

		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			//FFDebugMessage("GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT");
			return 0;

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			//FFDebugMessage("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
			return 0;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			//FFDebugMessage("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
			return 0;

		default:
			//FFDebugMessage("Unknown GL_FRAMEBUFFER error");
			return 0;		
	}

	return 1;
}

int FFGLFBO::UnbindAsRenderTarget(GLuint hostFbo )
{
	glBindFramebuffer(GL_FRAMEBUFFER, hostFbo);
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
	if (m_fboHandle)
	{
		glDeleteFramebuffers(1, &m_fboHandle);
		m_fboHandle = 0;
	}

	if (m_depthBufferHandle)
	{
		glDeleteRenderbuffersEXT(1, &m_depthBufferHandle);
		m_depthBufferHandle = 0;
	}

	if (m_glTextureHandle)
	{
		glDeleteTextures(1, &m_glTextureHandle);
		m_glTextureHandle = 0;
	}
}
