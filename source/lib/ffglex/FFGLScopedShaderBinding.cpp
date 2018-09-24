#include "FFGLScopedShaderBinding.h"

namespace ffglex
{
ScopedShaderBinding::ScopedShaderBinding( GLuint shaderID ) :
	isBound( true )
{
	glGetIntegerv( GL_CURRENT_PROGRAM, &previousBinding );
	glUseProgram( shaderID );
}
ScopedShaderBinding::~ScopedShaderBinding()
{
	EndScope();
}

void ScopedShaderBinding::EndScope()
{
	if( isBound )
	{
		glUseProgram( previousBinding );
		isBound = false;
	}
}

}//End namespace ffglex