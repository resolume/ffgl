#include "FFGLGradients.h"
#include <math.h>//floor
using namespace ffglex;

static CFFGLPluginInfo PluginInfo(
	PluginFactory< FFGLGradients >,// Create method
	"RS01",                        // Plugin unique ID
	"Gradient Example",            // Plugin name
	2,                             // API major version number
	1,                             // API minor version number
	1,                             // Plugin major version number
	000,                           // Plugin minor version number
	FF_SOURCE,                     // Plugin type
	"Sample FFGL Gradients plugin",// Plugin description
	"Resolume FFGL Example"        // About
);

static const char _vertexShaderCode[] = R"(#version 410 core
layout( location = 0 ) in vec4 vPosition;
layout( location = 1 ) in vec2 vUV;

out vec2 uv;

void main()
{
	gl_Position = vPosition;
	uv = vUV;
}
)";

static const char _fragmentShaderCode[] = R"(#version 410 core
uniform vec3 RGBLeft;
uniform vec3 RGBRight;

in vec2 uv;

out vec4 fragColor;

void main()
{
	fragColor = vec4( mix( RGBLeft, RGBRight, uv.x ), 1.0 );
}
)";

FFGLGradients::FFGLGradients() :
	rgbLeftLocation( -1 ),
	rgbRightLocation( -1 )
{
	// Input properties
	SetMinInputs( 0 );
	SetMaxInputs( 0 );

	// Parameters
	addParam( hue1 = Param::create( "Hue 1", 0.5f ) );
	addParam( hue2 = Param::create( "Hue 2" , 0.0f) );
	addParam( saturation = Param::create( "Saturation", 1.0f ) );
	addParam( brightness = Param::create( "Brightness", 1.0f ) );
}
FFResult FFGLGradients::InitGL( const FFGLViewportStruct* vp )
{
	if( !shader.Compile( _vertexShaderCode, _fragmentShaderCode ) )
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
	rgbLeftLocation  = shader.FindUniform( "RGBLeft" );
	rgbRightLocation = shader.FindUniform( "RGBRight" );

	//Use base-class init as success result so that it retains the viewport.
	return CFreeFrameGLPlugin::InitGL( vp );
}
FFResult FFGLGradients::ProcessOpenGL( ProcessOpenGLStruct* pGL )
{
	float rgb1[ 3 ];
	//we need to make sure the hue doesn't reach 1.0f, otherwise the result will be pink and not red how it should be
	float hue1Val = ( hue1->getValue() == 1.0f ) ? 0.0f : hue1->getValue();
	HSVtoRGB( hue1Val, saturation->getValue(), brightness->getValue(), rgb1[ 0 ], rgb1[ 1 ], rgb1[ 2 ] );

	float rgb2[ 3 ];
	float hue2Val = ( hue2->getValue() == 1.0f ) ? 0.0f : hue2->getValue();
	HSVtoRGB( hue2Val, saturation->getValue(), brightness->getValue(), rgb2[ 0 ], rgb2[ 1 ], rgb2[ 2 ] );

	//FFGL requires us to leave the context in a default state on return, so use this scoped binding to help us do that.
	ScopedShaderBinding shaderBinding( shader.GetGLID() );
	shader.Set( "RGBLeft" , rgb1[ 0 ], rgb1[ 1 ], rgb1[ 2 ] );
	shader.Set( "RGBRight" , rgb2[ 0 ], rgb2[ 1 ], rgb2[ 2 ] );

	quad.Draw();

	return FF_SUCCESS;
}
FFResult FFGLGradients::DeInitGL()
{
	shader.FreeGLResources();
	quad.Release();
	rgbLeftLocation  = -1;
	rgbRightLocation = -1;

	return FF_SUCCESS;
}