#include "FFGLShader.h"
#include <stdio.h>
#include <vector>
#include <assert.h>

#define LOGSHADERERRORS

namespace ffglex
{
/**
 * The default constructor just initializes this object to represent no shader.
 */
FFGLShader::FFGLShader() :
	vertexShaderID( 0 ),
	fragmentShaderID( 0 ),
	programID( 0 )
{
}
/**
 * Destructing this object just checks and lets you know if you've properly released the GL resources.
 */
FFGLShader::~FFGLShader()
{
	//If any of these assertions hit you forgot to free this shader's gl resources.
	assert( vertexShaderID == 0 );
	assert( fragmentShaderID == 0 );
	assert( programID == 0 );
}

/**
 * Compiles and links two shaders into a shader program.
 *
 * @param vertexShader: The glsl shader string that represents the shader that should be used in the vertex processing stage.
 * You should make sure that this glsl string defines the glsl version it's targetting (eg #version 330) and respect the OpenGL specs
 * for that shader version.
 * @param fragmentShader: The glsl shader string that represents the shader that should be used in the fragment processing stage.
 * You should make sure that this glsl string defines the glsl version it's targetting (eg #version 330) and respect the OpenGL specs
 * for that shader version.
 * @return: Whether or not compiling/linking succeeded and thus if this object can be used to bind a shader program for rendering.
 * If compilation succeeded you need to make sure that you're freeing the GL resources when you no longer intend to use this shader.
 */
bool FFGLShader::Compile( const std::string& vertexShader, const std::string& fragmentShader )
{
	return Compile( vertexShader.c_str(), fragmentShader.c_str() );
}
/**
 * Compiles and links two shaders into a shader program.
 *
 * @param vertexShader: The glsl shader string that represents the shader that should be used in the vertex processing stage.
 * You should make sure that this glsl string defines the glsl version it's targetting (eg #version 330) and respect the OpenGL specs
 * for that shader version.
 * @param fragmentShader: The glsl shader string that represents the shader that should be used in the fragment processing stage.
 * You should make sure that this glsl string defines the glsl version it's targetting (eg #version 330) and respect the OpenGL specs
 * for that shader version.
 * @return: Whether or not compiling/linking succeeded and thus if this object can be used to bind a shader program for rendering.
 * If compilation succeeded you need to make sure that you're freeing the GL resources when you no longer intend to use this shader.
 */
bool FFGLShader::Compile( const char* vertexShader, const char* fragmentShader )
{
	//We need to have valid inputs to be able to compile a shader, validate them here so that below we can just use them without checking.
	if( vertexShader == nullptr || strlen( vertexShader ) == 0 )
		return false;
	if( fragmentShader == nullptr || strlen( fragmentShader ) == 0 )
		return false;

	//Everything needs to succeed for this to be a usable shader. If anything fails we'll be cleaning up everything
	//so that we wont keep anything that wont be used alive.
	if( !CompileVertexShader( vertexShader ) ||
	    !CompileFragmentShader( fragmentShader ) ||
	    !LinkProgram() )
	{
		FreeGLResources();
		return false;
	}

	return true;
}
/**
 * Frees any GL resources that this shader is holding.
 *
 * Call this when you no longer intend to use this shader.
 */
void FFGLShader::FreeGLResources()
{
	if( vertexShaderID != 0 )
	{
		glDeleteShader( vertexShaderID );
		vertexShaderID = 0;
	}

	if( fragmentShaderID != 0 )
	{
		glDeleteShader( fragmentShaderID );
		fragmentShaderID = 0;
	}

	if( programID != 0 )
	{
		glDeleteProgram( programID );
		programID = 0;
	}
}

/**
 * Whether or not previous complilation succeeded and this shader is ready to be used for rendering.
 */
bool FFGLShader::IsReady() const
{
	return vertexShaderID != 0 && fragmentShaderID != 0 && programID != 0;
}
/**
 * Gets the OpenGL ID that represents the linked shader program.
 *
 * If the shader is usable this returns anything other than 0.
 */
GLuint FFGLShader::GetGLID() const
{
	return programID;
}
/**
 * Finds the uniform location that needs to be used to addess a uniform with a certain name.
 *
 * @param name: The name of the uniform for which you want to find the location. This name has to be
 * a uniform that's placed inside any of the shader strings provided to the previous successfull call to Compile.
 * @return: The uniform's location to be passed into a glUniform{1|2|3|4}{f|i} function call. -1 if no uniform with the
 * provided name is available.
 */
GLint FFGLShader::FindUniform( const char* name ) const
{
	return glGetUniformLocation( programID, name );
}

bool FFGLShader::CompileVertexShader( const char* vertexShader )
{
	vertexShaderID = glCreateShader( GL_VERTEX_SHADER );

	// Load Shader Sources
	glShaderSource( vertexShaderID, 1, &vertexShader, NULL );

	// Compile The Shaders
	glCompileShader( vertexShaderID );

	GLint compileStatus;
	glGetShaderiv( vertexShaderID, GL_COMPILE_STATUS, &compileStatus );

	if( compileStatus != GL_TRUE )
	{
#ifdef LOGSHADERERRORS
		GLint logLength;
		glGetShaderiv( vertexShaderID, GL_INFO_LOG_LENGTH, &logLength );
		std::vector< GLchar > log( logLength + 1 );
		memset( log.data(), 0, logLength + 1 );
		glGetShaderInfoLog( vertexShaderID, logLength, NULL, log.data() );

		printf( "Vertex Shader error: %s \n", log.data() );
#endif
	}

	return compileStatus == GL_TRUE;
}
bool FFGLShader::CompileFragmentShader( const char* vertexShader )
{
	fragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );

	// Load Shader Sources
	glShaderSource( fragmentShaderID, 1, &vertexShader, NULL );

	// Compile The Shaders
	glCompileShader( fragmentShaderID );

	GLint compileStatus;
	glGetShaderiv( fragmentShaderID, GL_COMPILE_STATUS, &compileStatus );

	if( compileStatus != GL_TRUE )
	{
#ifdef LOGSHADERERRORS
		GLint logLength;
		glGetShaderiv( fragmentShaderID, GL_INFO_LOG_LENGTH, &logLength );
		std::vector< GLchar > log( logLength + 1 );
		memset( log.data(), 0, logLength + 1 );
		glGetShaderInfoLog( fragmentShaderID, logLength, NULL, log.data() );

		printf( "Fragment Shader error: %s \n", log.data() );
#endif
	}

	return compileStatus == GL_TRUE;
}
bool FFGLShader::LinkProgram()
{
	programID = glCreateProgram();

	glAttachShader( programID, vertexShaderID );
	glAttachShader( programID, fragmentShaderID );

	GLint linkStatus = 0;
	glLinkProgram( programID );
	glGetProgramiv( programID, GL_LINK_STATUS, &linkStatus );

	if( linkStatus != GL_TRUE )
	{
#ifdef LOGSHADERERRORS
		//get the log so we can peek at the error string
		char log[ 1024 ];
		GLsizei returnedLength;
		glGetProgramInfoLog( fragmentShaderID, sizeof( log ) - 1, &returnedLength, log );
		log[ returnedLength ] = 0;

		printf( "Fragment Shader error: %s \n", log );
#endif
	}

	return linkStatus == GL_TRUE;
}

}//End namespace ffglex