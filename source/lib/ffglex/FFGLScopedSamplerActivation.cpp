#include "FFGLScopedSamplerActivation.h"

namespace ffglex
{
ScopedSamplerActivation::ScopedSamplerActivation( GLuint samplerIndex )
{
	glGetIntegerv( GL_ACTIVE_TEXTURE, &previousBinding );
	glActiveTexture( GL_TEXTURE0 + samplerIndex );
}
ScopedSamplerActivation::~ScopedSamplerActivation()
{
	glActiveTexture( previousBinding );
}

}//End namespace ffglex