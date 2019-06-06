#include "Effect.h"
using namespace ffglex;

Effect::Effect()
{
	SetMinInputs( 1 );
	SetMaxInputs( 1 );
	fragmentShaderCodeStart += R"(
		in vec2 i_uv;
		uniform sampler2D inputTexture;
	)";
	vertexShaderCode = R"(
		#version 410 core
		uniform vec2 maxUV;

		layout( location = 0 ) in vec4 vPosition;
		layout( location = 1 ) in vec2 vUV;

		out vec2 i_uv;

		void main()
		{
			gl_Position = vPosition;
			i_uv = vUV * maxUV;
		}
	)";
}

Effect::~Effect()
{
}

FFResult Effect::render( ProcessOpenGLStruct* inputTextures )
{
	if( inputTextures->numInputTextures < 1 )
		return FF_FAIL;
	if( inputTextures->inputTextures[ 0 ] == NULL )
		return FF_FAIL;

	shader.Use();
	shader.Bind("inputTexture", 0, *inputTextures->inputTextures[ 0 ] );
	FFGLTexCoords maxCoords = GetMaxGLTexCoords( *inputTextures->inputTextures[ 0 ] );
	shader.Set( "maxUV", maxCoords.s, maxCoords.t );
	quad.Draw();
	return FF_SUCCESS;
}