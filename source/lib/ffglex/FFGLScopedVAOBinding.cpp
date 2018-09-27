#include "FFGLScopedVAOBinding.h"

namespace ffglex
{
ScopedVAOBinding::ScopedVAOBinding( GLuint vaoID ) :
	isBound( true )
{
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
		glBindVertexArray( 0 );
		isBound = false;
	}
}

}//End namespace ffglex