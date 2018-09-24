#pragma once
#include "../ffgl/FFGL.h"//For OpenGL

namespace ffglex
{
class ScopedShaderBinding
{
public:
	ScopedShaderBinding( GLuint shaderID );
	~ScopedShaderBinding();

private:
	GLint previousBinding;
};

}//End namespace ffglex