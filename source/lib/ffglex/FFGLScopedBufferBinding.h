#pragma once
#include "../ffgl/FFGL.h"//For OpenGL

namespace ffglex
{
/**
 * The ScopedBufferBinding class provides a means of safely binding/unbinding an OpenGL
 * hardware buffer through means of RAII. On construction the provided buffer id will be
 * bound and on destruction it will be unbound, this way you can never forget to unbind your buffer.
 *
 * If you have know the type of your buffer and know where you want to bind it you can use the
 * ScopedVBOBinding, ScopedIBOBinding and ScopedUBOBinding classes to bind vertex buffers,
 * element array buffers and uniform buffers respectively.
 */
class ScopedBufferBinding
{
public:
	ScopedBufferBinding( GLenum target, GLuint newBinding );//Constructs this RAII binding which automatically binds your buffer to the requested target.
	virtual ~ScopedBufferBinding();                         //The destructor automatically unbinds the buffer if our scope wasn't ended manually.

	void EndScope();//Manually end the RAII scope. The first time you call this the buffer will be unbound, consecutive calls have no effect.

private:
	GLenum target;//!< The target to which the buffer object is bound.
	bool isBound; //!< Whether or not the buffer is currently bound. Maintained to provide the EndScope function without unbinding the buffer again in our destructor.
};

/**
 * This scoped buffer binding binds the provided buffer id as being a vertex buffer object.
 */
class ScopedVBOBinding : public ScopedBufferBinding
{
public:
	ScopedVBOBinding( GLuint vboID );
};
/**
 * This scoped buffer binding binds the provided buffer id as being an index buffer object. (OpenGL's element array buffer)
 */
class ScopedIBOBinding : public ScopedBufferBinding
{
public:
	ScopedIBOBinding( GLuint iboID );
};
/**
 * This scoped buffer binding binds the provided buffer id as being an uniform buffer object.
 */
class ScopedUBOBinding : public ScopedBufferBinding
{
public:
	ScopedUBOBinding( GLuint uboID );
};

}//End namespace ffglex