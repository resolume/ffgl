#include "FFGLMixer.h"
#include "../ffglex/FFGLScopedSamplerActivation.h"
#include "../ffglex/FFGLScopedTextureBinding.h"
#include "FFGLParam.h"
using namespace ffglex;

namespace ffglqs
{
Mixer::Mixer( bool supportTopLeftTextureOrientation ) :
	Plugin( supportTopLeftTextureOrientation )
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
	AddParam( Param::Create( "mixVal" ) );
}

Mixer::~Mixer()
{
}

FFResult Mixer::Render( ProcessOpenGLStruct* inputTextures )
{
	if( inputTextures->numInputTextures < 2 )
		return FF_FAIL;
	if( inputTextures->inputTextures[ 0 ] == nullptr )
		return FF_FAIL;
	if( inputTextures->inputTextures[ 1 ] == nullptr )
		return FF_FAIL;

	//Activate our shader using the scoped binding so that we'll restore the context state when we're done.
	ScopedShaderBinding shaderBinding( shader.GetGLID() );
	//Use the scoped bindings to ensure that the context will be returned in it's default state after we're done rendering.
	ScopedSamplerActivation activateSampler0( 0 );
	Scoped2DTextureBinding textureBinding0( inputTextures->inputTextures[ 0 ]->Handle );
	ScopedSamplerActivation activateSampler1( 1 );
	Scoped2DTextureBinding textureBinding1( inputTextures->inputTextures[ 1 ]->Handle );

	shader.Set( "textureDest", 0 );
	FFGLTexCoords maxCoords = GetMaxGLTexCoords( *inputTextures->inputTextures[ 0 ] );
	shader.Set( "maxUVDest", maxCoords.s, maxCoords.t );
	shader.Set( "textureSrc", 1 );
	maxCoords = GetMaxGLTexCoords( *inputTextures->inputTextures[ 1 ] );
	shader.Set( "maxUVSrc", maxCoords.s, maxCoords.t );
	quad.Draw();
	return FF_SUCCESS;
}

}//End namespace ffglqs