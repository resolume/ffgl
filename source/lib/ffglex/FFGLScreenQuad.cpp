#include "FFGLScreenQuad.h"
#include <assert.h>
#include "FFGLScopedVAOBinding.h"
#include "FFGLScopedBufferBinding.h"
#include "FFGLUtilities.h"

namespace ffglex
{

FFGLScreenQuad::FFGLScreenQuad() :
	vaoID( 0 ),
	vboID( 0 )
{
}
FFGLScreenQuad::~FFGLScreenQuad()
{
	//If any of these assertions hit you forgot to release this quad's gl resources.
	assert( vaoID == 0 );
	assert( vaoID == 0 );
}

bool FFGLScreenQuad::Initialise()
{
	glGenVertexArrays( 1, &vaoID );
	glGenBuffers( 1, &vboID );
	if( vaoID == 0 || vboID == 0 )
	{
		Release();
		return false;
	}

	ScopedVAOBinding vaoBinding( vaoID );
	ScopedVBOBinding vboBinding( vboID );
	glBufferData( GL_ARRAY_BUFFER, sizeof( TEXTURED_QUAD_VERTICES ), TEXTURED_QUAD_VERTICES, GL_STATIC_DRAW );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, false, sizeof( TEXTURED_QUAD_VERTICES[ 0 ] ), (char*)NULL + 2 * sizeof( float ) );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, false, sizeof( TEXTURED_QUAD_VERTICES[ 0 ] ), (char*)NULL + 0 * sizeof( float ) );

	vaoBinding.EndScope();

	glDisableVertexAttribArray( 0 );
	glDisableVertexAttribArray( 1 );
	return true;
}
void FFGLScreenQuad::Draw()
{
	ScopedVAOBinding vaoBinding( vaoID );
	glDrawArrays( GL_TRIANGLES, 0, 6 );
}
void FFGLScreenQuad::Release()
{
	glDeleteBuffers( 1, &vboID );
	vboID = 0;
	glDeleteVertexArrays( 1, &vaoID );
	vaoID = 0;
}

}//End namespace ffglex