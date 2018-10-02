#include "FFGLScopedBufferBinding.h"

namespace ffglex
{
ScopedBufferBinding::ScopedBufferBinding( GLenum target, GLuint newBinding ) :
	target( target ),
	isBound( true )
{
	glBindBuffer( target, newBinding );
}
ScopedBufferBinding::~ScopedBufferBinding()
{
	EndScope();
}

void ScopedBufferBinding::EndScope()
{
	if( isBound )
	{
		glBindBuffer( target, 0 );
		isBound = false;
	}
}

}//End namespace ffglex