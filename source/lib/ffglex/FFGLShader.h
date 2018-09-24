#pragma once
#include "../ffgl/FFGL.h"//For OpenGL
#include <string>

namespace ffglex
{
/**
 * The FFGLShader is a helper class you can use to compile glsl shader strings
 * and link them together into a shader program. You can then get the program id
 * and use it to bind the resulting shader to use it for the next rendering operation.
 */
class FFGLShader final
{
public:
	FFGLShader();                                                                        //The default constructor just initializes this object to represent no shader.
	FFGLShader( const FFGLShader& ) = delete;
	FFGLShader( FFGLShader&& ) = delete;
	~FFGLShader();                                                                       //Destructing this object just checks and lets you know if you've properly released the GL resources.

	bool Compile( const std::string& vertexShader, const std::string& fragmentShader );  //Compiles and links two shaders into a shader program.
	bool Compile( const char* vertexShader, const char* fragmentShader );                //Compiles and links two shaders into a shader program.
	void FreeGLResources();                                                              //Frees any GL resources that this shader is holding.

	bool IsReady() const;                                                                //Whether or not previous complilation succeeded and this shader is ready to be used for rendering.
	GLuint GetGLID() const;                                                              //Gets the OpenGL ID that represents the linked shader program.
	GLint FindUniform( const char* name ) const;                                         //Finds the uniform location that needs to be used to addess a uniform with a certain name.

	FFGLShader& operator=( const FFGLShader& ) = delete;
	FFGLShader& operator=( FFGLShader&& ) = delete;

private:
	bool CompileVertexShader( const char* vertexShader );    //Internal utility function that tries to compile a vertex shader and updates the internal vertexShaderID. Doesn't clean up on failure.
	bool CompileFragmentShader( const char* vertexShader );  //Internal utility function that tries to compile a fragmen shader and updates the internal fragmentShaderID. Doesn't clean up on failure.
	bool LinkProgram();                                      //Internal utility function that tries to link the two previously compiled vertex and fragment shaders into a program. Doesn't clean up on failure.

	GLuint vertexShaderID;    //!< The ID OpenGL gave our vertex shader. 0 for invalid.
	GLuint fragmentShaderID;  //!< The ID OpenGL gave our fragment shader. 0 for invalid.
	GLuint programID;         //!< The ID OpenGL gave our shader program. Bind this to use this shader. 0 for invalid.
};

}//End namespace ffglex