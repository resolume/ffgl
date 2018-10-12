#include "AddSubtract.h"
#include <ffgl/FFGLLib.h>
#include <ffglex/FFGLScopedShaderBinding.h>
#include <ffglex/FFGLScopedSamplerActivation.h>
#include <ffglex/FFGLScopedTextureBinding.h>
using namespace ffglex;

#define FFPARAM_BrightnessR ( 0 )
#define FFPARAM_BrightnessG ( 1 )
#define FFPARAM_BrightnessB ( 2 )

static CFFGLPluginInfo PluginInfo(
	PluginFactory< AddSubtract >,// Create method
	"RE01",                      // Plugin unique ID of maximum length 4.
	"AddSub Example",            // Plugin name
	2,                           // API major version number
	1,                           // API minor version number
	1,                           // Plugin major version number
	0,                           // Plugin minor version number
	FF_EFFECT,                   // Plugin type
	"Add and Subtract colours",  // Plugin description
	"Resolume FFGL Example"      // About
);

static const char vertexShaderCode[] = R"(#version 410 core
uniform vec2 MaxUV;

layout( location = 0 ) in vec4 vPosition;
layout( location = 1 ) in vec2 vUV;

out vec2 uv;

void main()
{
	gl_Position = vPosition;
	uv = vUV * MaxUV;
}
)";

static const char fragmentShaderCode[] = R"(#version 410 core
uniform sampler2D InputTexture;
uniform vec3 Brightness;

in vec2 uv;

out vec4 fragColor;

void main()
{
	vec4 color = texture( InputTexture, uv );
	//The InputTexture contains premultiplied colors, so we need to unpremultiply first to apply our effect on straight colors.
	if( color.a > 0.0 )
		color.rgb /= color.a;

	color.rgb = color.rgb + Brightness;

	//The plugin has to output premultiplied colors, this is how we're premultiplying our straight color while also
	//ensuring we aren't going out of the LDR the video engine is working in.
	color.rgb = clamp( color.rgb * color.a, vec3( 0.0 ), vec3( color.a ) );
	fragColor = color;
}
)";

AddSubtract::AddSubtract() :
	maxUVLocation( -1 ),
	brightnessLocation( -1 ),
	brightnessR( 0.5f ),
	brightnessG( 0.5f ),
	brightnessB( 0.5f )
{
	// Input properties
	SetMinInputs( 1 );
	SetMaxInputs( 1 );

	// Parameters
	SetParamInfof( FFPARAM_BrightnessR, "R", FF_TYPE_RED );
	SetParamInfof( FFPARAM_BrightnessG, "G", FF_TYPE_GREEN );
	SetParamInfof( FFPARAM_BrightnessB, "B", FF_TYPE_BLUE );
}
AddSubtract::~AddSubtract()
{
}

FFResult AddSubtract::InitGL( const FFGLViewportStruct* vp )
{
	if( !shader.Compile( vertexShaderCode, fragmentShaderCode ) )
	{
		DeInitGL();
		return FF_FAIL;
	}
	if( !quad.Initialise() )
	{
		DeInitGL();
		return FF_FAIL;
	}

	//FFGL requires us to leave the context in a default state on return, so use this scoped binding to help us do that.
	ScopedShaderBinding shaderBinding( shader.GetGLID() );

	//We're never changing the sampler to use, instead during rendering we'll make sure that we're always
	//binding the texture to sampler 0.
	glUniform1i( shader.FindUniform( "inputTexture" ), 0 );

	//We need to know these uniform locations because we need to set their value each frame.
	maxUVLocation = shader.FindUniform( "MaxUV" );
	brightnessLocation = shader.FindUniform( "Brightness" );

	//Use base-class init as success result so that it retains the viewport.
	return CFreeFrameGLPlugin::InitGL( vp );
}
FFResult AddSubtract::ProcessOpenGL( ProcessOpenGLStruct* pGL )
{
	if( pGL->numInputTextures < 1 )
		return FF_FAIL;

	if( pGL->inputTextures[ 0 ] == NULL )
		return FF_FAIL;

	//FFGL requires us to leave the context in a default state on return, so use this scoped binding to help us do that.
	ScopedShaderBinding shaderBinding( shader.GetGLID() );

	FFGLTextureStruct& Texture = *( pGL->inputTextures[ 0 ] );

	//The input texture's dimension might change each frame and so might the content area.
	//We're adopting the texture's maxUV using a uniform because that way we dont have to update our vertex buffer each frame.
	FFGLTexCoords maxCoords = GetMaxGLTexCoords( Texture );
	glUniform2f( maxUVLocation, maxCoords.s, maxCoords.t );

	glUniform3f( brightnessLocation,
				 -1.0f + ( brightnessR * 2.0f ),
				 -1.0f + ( brightnessG * 2.0f ),
				 -1.0f + ( brightnessB * 2.0f ) );

	//The shader's sampler is always bound to sampler index 0 so that's where we need to bind the texture.
	//Again, we're using the scoped bindings to help us keep the context in a default state.
	ScopedSamplerActivation activateSampler( 0 );
	Scoped2DTextureBinding textureBinding( Texture.Handle );

	quad.Draw();

	return FF_SUCCESS;
}
FFResult AddSubtract::DeInitGL()
{
	shader.FreeGLResources();
	quad.Release();
	maxUVLocation = -1;
	brightnessLocation = -1;

	return FF_SUCCESS;
}

FFResult AddSubtract::SetFloatParameter( unsigned int dwIndex, float value )
{
	switch( dwIndex )
	{
	case FFPARAM_BrightnessR:
		brightnessR = value;
		break;
	case FFPARAM_BrightnessG:
		brightnessG = value;
		break;
	case FFPARAM_BrightnessB:
		brightnessB = value;
		break;
	default:
		return FF_FAIL;
	}

	return FF_SUCCESS;
}

float AddSubtract::GetFloatParameter( unsigned int dwIndex )
{
	switch( dwIndex )
	{
	case FFPARAM_BrightnessR:
		return brightnessR;
	case FFPARAM_BrightnessG:
		return brightnessG;
	case FFPARAM_BrightnessB:
		return brightnessB;

	default:
		return 0.0f;
	}
}
