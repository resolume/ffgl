#include "FFGLScopedBufferBinding.h"

namespace ffglex
{

/**
 * Constructs this RAII binding which automatically binds your buffer to the requested target.
 *
 * @param target: The buffer target to which you want to bind your buffer. See glBindBuffer
 * for possible values of this argument.
 * @param newBinding: The id of the buffer object you want to bind.
 */
ScopedBufferBinding::ScopedBufferBinding( GLenum target, GLuint newBinding ) :
	target( target ),
	isBound( true )
{
	glBindBuffer( target, newBinding );
}
/**
 * The destructor automatically unbinds the buffer if our scope wasn't ended manually.
 */
ScopedBufferBinding::~ScopedBufferBinding()
{
	EndScope();
}

/**
 * Manually end the RAII scope. The first time you call this the buffer will be unbound, consecutive calls have no effect.
 */
void ScopedBufferBinding::EndScope()
{
	if( isBound )
	{
		glBindBuffer( target, 0 );
		isBound = false;
	}
}


ScopedVBOBinding::ScopedVBOBinding( GLuint vboID ) :
	ScopedBufferBinding( GL_ARRAY_BUFFER, vboID )
{
}


ScopedIBOBinding::ScopedIBOBinding( GLuint iboID ) :
	ScopedBufferBinding( GL_ELEMENT_ARRAY_BUFFER, iboID )
{
}


ScopedUBOBinding::ScopedUBOBinding( GLuint uboID ) :
	ScopedBufferBinding( GL_UNIFORM_BUFFER, uboID )
{
}

}//End namespace ffglex