#pragma once
#include "../ffgl/FFGL.h"//For OpenGL

namespace ffglex
{
class ScopedShaderBinding
{
public:
	ScopedShaderBinding( GLuint shaderID );
	~ScopedShaderBinding();

	void EndScope();

private:
	GLint previousBinding;
	bool isBound;
};

}//End namespace ffglex