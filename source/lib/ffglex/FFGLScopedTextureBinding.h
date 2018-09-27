#pragma once
#include "../ffgl/FFGL.h"//For OpenGL

namespace ffglex
{
class ScopedTextureBinding
{
public:
	ScopedTextureBinding( GLenum target, GLenum bindingName, GLuint textureID );
	virtual ~ScopedTextureBinding();

	void EndScope();

private:
	GLenum target;
	GLint previousBinding;
	bool isBound;
};

class Scoped2DTextureBinding : public ScopedTextureBinding
{
public:
	Scoped2DTextureBinding( GLuint textureID ) :
		ScopedTextureBinding( GL_TEXTURE_2D, GL_TEXTURE_BINDING_2D, textureID )
	{
	}
};

}//End namespace ffglex