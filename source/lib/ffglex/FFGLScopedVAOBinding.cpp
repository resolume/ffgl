#include "FFGLScopedVAOBinding.h"

namespace ffglex
{
ScopedVAOBinding::ScopedVAOBinding( GLuint vaoID )
{
	glGetIntegerv( GL_VERTEX_ARRAY_BINDING, &previousBinding );
	glBindVertexArray( vaoID );
}
ScopedVAOBinding::~ScopedVAOBinding()
{
	glBindVertexArray( previousBinding );
}

}//End namespace ffglex