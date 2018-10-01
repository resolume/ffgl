#include "FFGLScopedTextureBinding.h"

namespace ffglex
{
ScopedTextureBinding::ScopedTextureBinding( GLenum target, GLuint textureID ) :
	target( target ),
	isBound( true )
{
	glBindTexture( target, textureID );
}
ScopedTextureBinding::~ScopedTextureBinding()
{
	EndScope();
}

void ScopedTextureBinding::EndScope()
{
	if( isBound )
	{
		glBindTexture( target, 0 );
		isBound = false;
	}
}

}//End namespace ffglex