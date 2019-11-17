#pragma once
#include "../ffgl/FFGL.h"//For OpenGL
#include <vector>
#include <string>
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
	FFGLShader();                                                                                     //The default constructor just initializes this object to represent no shader.
	~FFGLShader();                                                                                    //Destructing this object just checks and lets you know if you've properly released the GL resources.

	void AddTransformFeedbackVarying( const std::string& varyingName );                               //Add a varying that will be captured using a transform feedback.
	bool Compile( const std::string& vertexShader, const std::string& fragmentShader );               //Compiles and links two shaders into a shader program.
	bool Compile( const char* vertexShader, const char* fragmentShader );                             //Compiles and links two shaders into a shader program.
	bool Compile( const char* vertexShader, const char* geometryShader, const char* fragmentShader ); //Compiles and links a vertex/geometry/fragment shader into a single shader program.
	void FreeGLResources();                                                                           //Frees any GL resources that this shader is holding.

	void Set( const char* name, float value );
	void Set( const char* name, float v1, float v2 );
	void Set( const char* name, float v1, float v2, float v3 );
	void Set( const char* name, float v1, float v2, float v3, float v4 );
	void Set( const char* name, int value );

	bool IsReady() const;                                                                             //Whether or not previous complilation succeeded and this shader is ready to be used for rendering.
	GLuint GetGLID() const;                                                                           //Gets the OpenGL ID that represents the linked shader program.
	GLint FindUniform( const char* name ) const;                                                      //Finds the uniform location that needs to be used to addess a uniform with a certain name.

	FFGLShader& operator=( const FFGLShader& ) = delete;
	FFGLShader& operator=( FFGLShader&& ) = delete;

private:
	bool CompileVertexShader( const char* vertexShader );    //Internal utility function that tries to compile a vertex shader and updates the internal vertexShaderID. Doesn't clean up on failure.
	bool CompileGeometryShader( const char* geometryShader );//Internal utility function that tries to compile a geometry shader and updates the internal geometryShaderID. Doesn't clean up on failure.
	bool CompileFragmentShader( const char* fragmentShader );//Internal utility function that tries to compile a fragment shader and updates the internal fragmentShaderID. Doesn't clean up on failure.
	bool LinkProgram();                                      //Internal utility function that tries to link the two previously compiled vertex and fragment shaders into a program. Doesn't clean up on failure.

	GLuint vertexShaderID;                                //!< The ID OpenGL gave our vertex shader. 0 for invalid.
	GLuint geometryShaderID;                              //!< The ID OpenGL gave our geometry shader. 0 if not present or invalid.
	GLuint fragmentShaderID;                              //!< The ID OpenGL gave our fragment shader. 0 for invalid.
	GLuint programID;                                     //!< The ID OpenGL gave our shader program. Bind this to use this shader. 0 for invalid.
	std::vector< std::string > transformFeedbackVaryings; //!< The varyings that will be captured using a transform feedback. Ordered in the order of capturing.
};

}//End namespace ffglex