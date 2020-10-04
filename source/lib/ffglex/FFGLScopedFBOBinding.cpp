#include "FFGLScopedFBOBinding.h"

namespace ffglex
{
ScopedFBOBinding::ScopedFBOBinding( GLuint fboID, RestoreBehaviour restoreBehaviour ) :
	isBound( true ),
	restoreBehaviour( restoreBehaviour ),
	previousDrawFboID( 0 ),
	previosReadFboID( 0 )
{
	//If we have to revert to the currently bound fbo we need to get their id's. If we dont get these id's we'll automatically
	//get the clearing behaviour as we've initialized the fboIDs to 0.
	if( restoreBehaviour == RB_REVERT )
	{
		glGetIntegerv( GL_DRAW_FRAMEBUFFER_BINDING, &previousDrawFboID );
		glGetIntegerv( GL_READ_FRAMEBUFFER_BINDING, &previosReadFboID );
	}

	glBindFramebuffer( GL_FRAMEBUFFER, fboID );
}
ScopedFBOBinding::~ScopedFBOBinding()
{
	EndScope();
}

void ScopedFBOBinding::EndScope()
{
	if( isBound )
	{
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, previousDrawFboID );
		glBindFramebuffer( GL_READ_FRAMEBUFFER, previosReadFboID );
		isBound = false;
	}
}

}//End namespace ffglex