#include "FFGLScopedRenderBufferBinding.h"

namespace ffglex
{
ScopedRenderBufferBinding::ScopedRenderBufferBinding( GLuint renderbufferID ) :
	isBound( true )
{
	glBindRenderbuffer( GL_RENDERBUFFER, renderbufferID );
}
ScopedRenderBufferBinding::~ScopedRenderBufferBinding()
{
	EndScope();
}

void ScopedRenderBufferBinding::EndScope()
{
	if( isBound )
	{
		glBindRenderbuffer( GL_RENDERBUFFER, 0 );
		isBound = false;
	}
}

}//End namespace ffglex