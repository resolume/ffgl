#include "FFGLScopedSamplerActivation.h"

namespace ffglex
{
ScopedSamplerActivation::ScopedSamplerActivation( GLuint samplerIndex ) :
	isBound( true )
{
	glGetIntegerv( GL_ACTIVE_TEXTURE, &previousBinding );
	glActiveTexture( GL_TEXTURE0 + samplerIndex );
}
ScopedSamplerActivation::~ScopedSamplerActivation()
{
	EndScope();
}

void ScopedSamplerActivation::EndScope()
{
	if( isBound )
	{
		glActiveTexture( previousBinding );
		isBound = false;
	}
}

}//End namespace ffglex