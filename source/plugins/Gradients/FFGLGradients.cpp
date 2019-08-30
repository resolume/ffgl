#include "FFGLGradients.h"
#include <math.h>//floor
using namespace ffglex;

#define FFPARAM_Hue1 ( 0 )
#define FFPARAM_Hue2 ( 1 )
#define FFPARAM_Saturation ( 2 )
#define FFPARAM_Brightness ( 3 )

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

static const char vertexShaderCode[] = R"(#version 410 core
layout( location = 0 ) in vec4 vPosition;
layout( location = 1 ) in vec2 vUV;

out vec2 uv;

void main()
{
	gl_Position = vPosition;
	uv = vUV;
}
)";

static const char fragmentShaderCode[] = R"(#version 410 core
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
	rgbRightLocation( -1 ),
	m_Hue1( 0.0f ),
	m_Hue2( 0.5f ),
	m_Saturation( 1.0f ),
	m_Brightness( 1.0f )
{
	// Input properties
	SetMinInputs( 0 );
	SetMaxInputs( 0 );

	// Parameters
	SetParamInfof( FFPARAM_Hue1, "Hue 1", FF_TYPE_STANDARD );
	SetParamInfof( FFPARAM_Hue2, "Hue 2", FF_TYPE_STANDARD );
	SetParamInfof( FFPARAM_Saturation, "Saturation", FF_TYPE_STANDARD );
	SetParamInfof( FFPARAM_Brightness, "Brightness", FF_TYPE_STANDARD );
}
FFResult FFGLGradients::InitGL( const FFGLViewportStruct* vp )
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
	rgbLeftLocation  = shader.FindUniform( "RGBLeft" );
	rgbRightLocation = shader.FindUniform( "RGBRight" );

	//Use base-class init as success result so that it retains the viewport.
	return CFreeFrameGLPlugin::InitGL( vp );
}
FFResult FFGLGradients::ProcessOpenGL( ProcessOpenGLStruct* pGL )
{
	float rgb1[ 3 ];
	//we need to make sure the hue doesn't reach 1.0f, otherwise the result will be pink and not red how it should be
	float hue1 = ( m_Hue1 == 1.0f ) ? 0.0f : m_Hue1;
	HSVtoRGB( hue1, m_Saturation, m_Brightness, rgb1[ 0 ], rgb1[ 1 ], rgb1[ 2 ] );

	float rgb2[ 3 ];
	float hue2 = ( m_Hue2 == 1.0f ) ? 0.0f : m_Hue2;
	HSVtoRGB( hue2, m_Saturation, m_Brightness, rgb2[ 0 ], rgb2[ 1 ], rgb2[ 2 ] );

	//FFGL requires us to leave the context in a default state on return, so use this scoped binding to help us do that.
	ScopedShaderBinding shaderBinding( shader.GetGLID() );
	glUniform3f( rgbLeftLocation, rgb1[ 0 ], rgb1[ 1 ], rgb1[ 2 ] );
	glUniform3f( rgbRightLocation, rgb2[ 0 ], rgb2[ 1 ], rgb2[ 2 ] );

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

FFResult FFGLGradients::SetFloatParameter( unsigned int dwIndex, float value )
{
	switch( dwIndex )
	{
	case FFPARAM_Hue1:
		m_Hue1 = value;
		break;
	case FFPARAM_Hue2:
		m_Hue2 = value;
		break;
	case FFPARAM_Saturation:
		m_Saturation = value;
		break;
	case FFPARAM_Brightness:
		m_Brightness = value;
		break;
	default:
		return FF_FAIL;
	}

	return FF_SUCCESS;
}

float FFGLGradients::GetFloatParameter( unsigned int index )
{
	float retValue = 0.0;

	switch( index )
	{
	case FFPARAM_Hue1:
		retValue = m_Hue1;
		break;
	case FFPARAM_Hue2:
		retValue = m_Hue2;
		break;
	case FFPARAM_Saturation:
		retValue = m_Saturation;
		break;
	case FFPARAM_Brightness:
		retValue = m_Brightness;
		break;
	default:
		break;
	}

	return retValue;
}
