#include "FFGLScopedTextureBinding.h"

namespace ffglex
{
ScopedTextureBinding::ScopedTextureBinding( GLenum target, GLenum bindingName, GLuint textureID ) :
	target( target ),
	isBound( true )
{
	glGetIntegerv( bindingName, &previousBinding );
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
		glBindTexture( target, previousBinding );
		isBound = false;
	}
}

}//End namespace ffglex