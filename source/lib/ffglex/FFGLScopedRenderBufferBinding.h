#pragma once
#include "../ffgl/FFGL.h"//For OpenGL

namespace ffglex
{
class ScopedRenderBufferBinding
{
public:
	ScopedRenderBufferBinding( GLuint renderbufferID );
	~ScopedRenderBufferBinding();

	void EndScope();

private:
	bool isBound;
};

}//End namespace ffglex