#pragma once
#include "../ffgl/FFGL.h"//For OpenGL

namespace ffglex
{
class ScopedSamplerActivation
{
public:
	ScopedSamplerActivation( GLuint samplerIndex );
	~ScopedSamplerActivation();

private:
	GLint previousBinding;
};

}//End namespace ffglex