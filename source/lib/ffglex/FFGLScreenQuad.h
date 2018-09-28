#pragma once
#include "../ffgl/FFGL.h"//For OpenGL
#include <string>

namespace ffglex
{
/**
 * The ScreenQuad is a utility that helps you load vertex data representing a quad into a buffer
 * and setting up a vao to source it's vertex attributes from this buffer. You can then tell this
 * quad to draw itself which will use that buffer and vao to render the quad.
 */
class FFGLScreenQuad
{
public:
	FFGLScreenQuad();
	FFGLScreenQuad( const FFGLScreenQuad& ) = delete;
	FFGLScreenQuad( FFGLScreenQuad&& ) = delete;
	~FFGLScreenQuad();

	bool Initialise();       //Allow this utility to load the data it requires to do it's rendering into it's buffers.
	void Draw();             //Draw the quad. Depending on your vertex shader this will apply your fragment shader in the area where the quad ends up.
	void Release();          //Release the gpu resources this quad has loaded into vram. Call this before destruction if you've previously initialised us.

private:
	GLuint vaoID;
	GLuint vboID;
};

}//End namespace ffglex