#include "FFGLScopedShaderBinding.h"

namespace ffglex
{
ScopedShaderBinding::ScopedShaderBinding( GLuint shaderID ) :
	isBound( true )
{
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
		glUseProgram( 0 );
		isBound = false;
	}
}

}//End namespace ffglex