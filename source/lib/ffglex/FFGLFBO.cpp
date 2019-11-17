#include "FFGLFBO.h"
#include "FFGLScopedFBOBinding.h"
#include "FFGLScopedRenderBufferBinding.h"
#include "FFGLScopedTextureBinding.h"

namespace ffglex
{
FFGLFBO::FFGLFBO() :
	width( 0 ),
	height( 0 ),
	internalColorFormat( 0 ),
	fboID( 0 ),
	depthBufferID( 0 ),
	colorTextureID( 0 )
{
}

bool FFGLFBO::Initialise( GLsizei width, GLsizei height, GLint internalColorFormat )
{
	//Cannot initialize if we're already initialized, please Release us first.
	if( fboID != 0 )
		return false;

	this->width = width;
	this->height = height;
	this->internalColorFormat = internalColorFormat;

	//If any of the generation functions fail we'll release the resources that did successfully initialize so that
	//we wont end up in a partially initialized state.
	if( !GenerateFBO() || !GenerateDepthBuffer() || !GenerateColorTexture() )
	{
		Release();
		return false;
	}

	//If we got to this point generating the depth buffer and color textures has succeeded so we can now attach them to the fbo.
	//We use a reverting binding behaviour so that the bound fboIDs may be reverted back to the host's fbo.
	ScopedFBOBinding scopedFBO( fboID, ScopedFBOBinding::RB_REVERT );
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTextureID, 0 );

	//If the framebuffer status is not complete for some reason we'll release all the resources we've initialized so that
	//they dont keep dangling around. The caller should not have to call Release if Initialise failed.
	if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
	{
		Release();
		return false;
	}

	return true;
}
void FFGLFBO::Release()
{
	width = 0;
	height = 0;

	if( fboID != 0 )
	{
		glDeleteFramebuffers( 1, &fboID );
		fboID = 0;
	}

	if( depthBufferID != 0 )
	{
		glDeleteRenderbuffers( 1, &depthBufferID );
		depthBufferID = 0;
	}

	if( depthBufferID != 0 )
	{
		glDeleteTextures( 1, &depthBufferID );
		depthBufferID = 0;
	}
}

void FFGLFBO::ResizeViewPort()
{
	glViewport( 0, 0, width, height );
}

GLuint FFGLFBO::GetWidth() const
{
	return width;
}
GLuint FFGLFBO::GetHeight() const
{
	return height;
}
GLuint FFGLFBO::GetGLID() const
{
	return fboID;
}
FFGLTextureStruct FFGLFBO::GetTextureInfo() const
{
	FFGLTextureStruct t;

	t.Width = t.HardwareWidth = width;
	t.Height = t.HardwareHeight = height;

	t.Handle = colorTextureID;

	return t;
}

bool FFGLFBO::GenerateFBO()
{
	glGenFramebuffers( 1, &fboID );
	return fboID != 0;
}
bool FFGLFBO::GenerateDepthBuffer()
{
	glGenRenderbuffers( 1, &depthBufferID );
	if( depthBufferID == 0 )
		return false;

	ScopedRenderBufferBinding scopedBinding( depthBufferID );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height );
	return true;
}
bool FFGLFBO::GenerateColorTexture()
{
	glGenTextures( 1, &colorTextureID );
	if( colorTextureID == 0 )
		return false;

	//Temporarily bind the texture so that we may initialize it's size.
	Scoped2DTextureBinding textureBinding( colorTextureID );

	//Create the texture with no mipmap levels and no borders. The data format and data type dont really matter as we're not uploading any data.
	glTexImage2D( GL_TEXTURE_2D, 0, internalColorFormat, width, height, 0,
	              GL_RGBA, internalColorFormat == GL_RGBA8 ? GL_UNSIGNED_BYTE : GL_FLOAT, NULL );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	return true;
}

}//End namespace ffglex
