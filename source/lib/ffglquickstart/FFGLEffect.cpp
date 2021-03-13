#include "FFGLEffect.h"
#include "../ffglex/FFGLScopedSamplerActivation.h"
#include "../ffglex/FFGLScopedTextureBinding.h"
using namespace ffglex;

namespace ffglqs
{
Effect::Effect( bool supportTopLeftTextureOrientation ) :
	Plugin( supportTopLeftTextureOrientation )
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

FFResult Effect::Render( ProcessOpenGLStruct* inputTextures )
{
	if( inputTextures->numInputTextures < 1 )
		return FF_FAIL;
	if( inputTextures->inputTextures[ 0 ] == NULL )
		return FF_FAIL;

	//Activate our shader using the scoped binding so that we'll restore the context state when we're done.
	ScopedShaderBinding shaderBinding( shader.GetGLID() );
	//Use the scoped bindings to ensure that the context will be returned in it's default state after we're done rendering.
	ScopedSamplerActivation activateSampler0( 0 );
	Scoped2DTextureBinding textureBinding0( inputTextures->inputTextures[ 0 ]->Handle );

	shader.Set( "inputTexture", 0 );
	FFGLTexCoords maxCoords = GetMaxGLTexCoords( *inputTextures->inputTextures[ 0 ] );
	shader.Set( "maxUV", maxCoords.s, maxCoords.t );
	quad.Draw();
	return FF_SUCCESS;
}

}//End namespace ffglqs