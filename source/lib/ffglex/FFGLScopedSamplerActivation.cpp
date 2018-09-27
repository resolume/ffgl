#include "FFGLScopedSamplerActivation.h"

namespace ffglex
{
ScopedSamplerActivation::ScopedSamplerActivation( GLuint samplerIndex ) :
	isBound( true )
{
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
		glActiveTexture( 0 );
		isBound = false;
	}
}

}//End namespace ffglex