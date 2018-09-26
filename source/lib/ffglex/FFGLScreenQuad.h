#pragma once
#include "../ffgl/FFGL.h"//For OpenGL
#include <string>

namespace ffglex
{
class FFGLScreenQuad
{
public:
	FFGLScreenQuad();
	FFGLScreenQuad( const FFGLScreenQuad& ) = delete;
	FFGLScreenQuad( FFGLScreenQuad&& ) = delete;
	~FFGLScreenQuad();

	bool Initialise();
	void Draw();
	void Release();

private:
	GLuint vaoID;
	GLuint vboID;
};

}//End namespace ffglex