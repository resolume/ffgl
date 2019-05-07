#include "Mixer.h"
using namespace ffglex;

Mixer::Mixer()
{
	SetMinInputs( 2 );
	SetMaxInputs( 2 );
	fragmentShaderCodeStart += R"(
		uniform sampler2D textureDest;
		uniform sampler2D textureSrc;

		in vec2 i_uv_dest;
		in vec2 i_uv_src;
	)";
	vertexShaderCode = R"(#version 410 core
		uniform vec2 maxUVDest;
		uniform vec2 maxUVSrc;

		layout( location = 0 ) in vec4 vPosition;
		layout( location = 1 ) in vec2 vUV;

		out vec2 i_uv_dest;
		out vec2 i_uv_src;

		void main()
		{
			gl_Position = vPosition;
			i_uv_dest = vUV * maxUVDest;
			i_uv_src = vUV * maxUVSrc;
		}
	)";
	addParam( Param::create( "mixVal" ) );
}

Mixer::~Mixer()
{
}

FFResult Mixer::render( ProcessOpenGLStruct* inputTextures )
{
	if( inputTextures->numInputTextures < 2 )
		return FF_FAIL;
	if( inputTextures->inputTextures[ 0 ] == nullptr )
		return FF_FAIL;
	if( inputTextures->inputTextures[ 1 ] == nullptr )
		return FF_FAIL;

	shader.Use();
	shader.Bind("textureDest", 0, *inputTextures->inputTextures[ 0 ] );
	FFGLTexCoords maxCoords = GetMaxGLTexCoords( *inputTextures->inputTextures[ 0 ] );
	shader.Set( "maxUVDest", maxCoords.s, maxCoords.t );
	shader.Bind( "textureSrc", 1, *inputTextures->inputTextures[ 1 ] );
	maxCoords = GetMaxGLTexCoords( *inputTextures->inputTextures[ 1 ] );
	shader.Set( "maxUVSrc", maxCoords.s, maxCoords.t );
	quad.Draw();
	return FF_SUCCESS;
}