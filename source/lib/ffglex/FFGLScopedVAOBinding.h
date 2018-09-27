#pragma once
#include "../ffgl/FFGL.h"//For OpenGL

namespace ffglex
{
class ScopedVAOBinding
{
public:
	ScopedVAOBinding( GLuint vaoID );
	~ScopedVAOBinding();

	void EndScope();

private:
	bool isBound;
};

}//End namespace ffglex