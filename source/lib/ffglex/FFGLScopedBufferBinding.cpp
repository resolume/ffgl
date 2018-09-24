#include "FFGLScopedBufferBinding.h"

namespace ffglex
{
ScopedBufferBinding::ScopedBufferBinding( GLenum target, GLenum bindingName, GLuint newBinding ) :
	target( target ),
	previousBinding( 0 )
{
	glGetIntegerv( bindingName, &previousBinding );
	glBindBuffer( target, newBinding );
}
ScopedBufferBinding::~ScopedBufferBinding()
{
	glBindBuffer( target, previousBinding );
}

}//End namespace ffglex