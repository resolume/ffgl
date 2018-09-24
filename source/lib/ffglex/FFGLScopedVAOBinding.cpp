#include "FFGLScopedVAOBinding.h"

namespace ffglex
{
ScopedVAOBinding::ScopedVAOBinding( GLuint vaoID ) :
	isBound( true )
{
	glGetIntegerv( GL_VERTEX_ARRAY_BINDING, &previousBinding );
	glBindVertexArray( vaoID );
}
ScopedVAOBinding::~ScopedVAOBinding()
{
	EndScope();
}

void ScopedVAOBinding::EndScope()
{
	if( isBound )
	{
		glBindVertexArray( previousBinding );
		isBound = false;
	}
}

}//End namespace ffglex