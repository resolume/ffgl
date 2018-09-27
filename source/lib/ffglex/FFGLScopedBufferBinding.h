#pragma once
#include "../ffgl/FFGL.h"//For OpenGL

namespace ffglex
{
class ScopedBufferBinding
{
public:
	ScopedBufferBinding( GLenum target, GLuint newBinding );
	virtual ~ScopedBufferBinding();

	void EndScope();

private:
	GLenum target;
	bool isBound;
};

class ScopedVBOBinding : public ScopedBufferBinding
{
public:
	ScopedVBOBinding( GLuint vboID ) :
		ScopedBufferBinding( GL_ARRAY_BUFFER, vboID )
	{
	}
};

}//End namespace ffglex