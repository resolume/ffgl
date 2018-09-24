#pragma once
#include "../ffgl/FFGL.h"//For OpenGL

namespace ffglex
{
class ScopedBufferBinding
{
public:
	ScopedBufferBinding( GLenum target, GLenum bindingName, GLuint newBinding );
	virtual ~ScopedBufferBinding();

private:
	GLenum target;
	GLint previousBinding;
};

class ScopedVBOBinding : public ScopedBufferBinding
{
public:
	ScopedVBOBinding( GLuint vboID ) :
		ScopedBufferBinding( GL_ARRAY_BUFFER, GL_ARRAY_BUFFER_BINDING, vboID )
	{
	}
};

}//End namespace ffglex