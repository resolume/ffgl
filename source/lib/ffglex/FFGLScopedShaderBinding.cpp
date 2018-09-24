#include "FFGLScopedShaderBinding.h"

namespace ffglex
{
ScopedShaderBinding::ScopedShaderBinding( GLuint shaderID )
{
	glGetIntegerv( GL_CURRENT_PROGRAM, &previousBinding );
	glUseProgram( shaderID );
}
ScopedShaderBinding::~ScopedShaderBinding()
{
	glUseProgram( previousBinding );
}

}//End namespace ffglex