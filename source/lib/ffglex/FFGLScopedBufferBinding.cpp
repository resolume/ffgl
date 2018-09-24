#include "FFGLScopedBufferBinding.h"

namespace ffglex
{
ScopedBufferBinding::ScopedBufferBinding( GLenum target, GLenum bindingName, GLuint newBinding ) :
	target( target ),
	previousBinding( 0 ),
	isBound( true )
{
	glGetIntegerv( bindingName, &previousBinding );
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
		glBindBuffer( target, previousBinding );
		isBound = false;
	}
}

}//End namespace ffglex