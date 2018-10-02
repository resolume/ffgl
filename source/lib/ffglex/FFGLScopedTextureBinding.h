#pragma once
#include "../ffgl/FFGL.h"//For OpenGL

namespace ffglex
{
class ScopedTextureBinding
{
public:
	ScopedTextureBinding( GLenum target, GLuint textureID );
	virtual ~ScopedTextureBinding();

	void EndScope();

private:
	GLenum target;
	bool isBound;
};

class Scoped2DTextureBinding : public ScopedTextureBinding
{
public:
	Scoped2DTextureBinding( GLuint textureID ) :
		ScopedTextureBinding( GL_TEXTURE_2D, textureID )
	{
	}
};

}//End namespace ffglex