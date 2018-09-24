#pragma once
#include "../ffgl/FFGL.h"//For OpenGL

namespace ffglex
{
class ScopedVAOBinding
{
public:
	ScopedVAOBinding( GLuint vaoID );
	~ScopedVAOBinding();

private:
	GLint previousBinding;
};

}//End namespace ffglex