#pragma once
#include "../ffgl/FFGL.h"//For OpenGL

namespace ffglex
{
class ScopedSamplerActivation
{
public:
	ScopedSamplerActivation( GLuint samplerIndex );
	~ScopedSamplerActivation();

	void EndScope();

private:
	bool isBound;
};

}//End namespace ffglex