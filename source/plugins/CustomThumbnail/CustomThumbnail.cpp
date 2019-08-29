#include "CustomThumbnail.h"
#include <math.h>//floor
using namespace ffglex;

enum ParamID
{
	PID_INTENSITY
};

/**
 * There's two ways to provide thumbnails to the host. We could generate one when the plugin is first loaded, or we could have the raw rgba data embedded in the plugin.
 * This define is used to choose between the two modes. If this define is enabled this example shows how to generate a thumbnail when the plugin is being loaded.
 * To see the example where it uses embedded rgba data remove this define.
 */
#define USE_DYNAMIC_THUMBNAIL

static CFFGLPluginInfo PluginInfo(
	PluginFactory< CustomThumbnail >,           // Creation method
	"RS03",                                     // Plugin unique ID
	"Custom Thumbnail Example",                 // Plugin name
	2,                                          // API major version number
	1,                                          // API minor version number
	1,                                          // Plugin major version number
	000,                                        // Plugin minor version number
	FF_SOURCE,                                  // Plugin type
	"Sample FFGL plugin with custom thumbnail", // Plugin description
	"Resolume FFGL Example"                     // About
);

#if defined( USE_DYNAMIC_THUMBNAIL )
/**
 * The plugin gets to decide how large it's thumbnail is, it's then up to the host to determine how to represent this thumbnail
 * on the screen. Some hosts may crop the thumbnails, some may stretch it, some apply autosizing to fit the thumbnail.
 * Resolume uses 160x120 thumbnails, and since we're testing with Resolume being the host we're matching this resolution so that
 * we're getting a pixel perfect thumbnail.
 */
static const FFUInt32 THUMBNAIL_WIDTH = 160;
static const FFUInt32 THUMBNAIL_HEIGHT = 120;
std::vector< Color > generateThumbnail()
{
	/**
	 * NOTE: There's no active context here and the plugin has not been initialized. This function can only be used to generate a thumbnail on the CPU.
	 * You may want to use a function like this one, if the thumbnail you've embedded has some encoding applied. You can use this function to decode, eg
	 * decode from png to raw RGBA.
	 */
	std::vector< Color > thumbnail( THUMBNAIL_WIDTH * THUMBNAIL_HEIGHT );
	for( Color& color : thumbnail )
	{
		unsigned char intensity = rand() % 256;
		color = Color( intensity, intensity, intensity, 255 );
	}
	return thumbnail;
}
/**
 * The default thumbnail rendered by the host is kind of boring as we've set our intensity to 0 by default so it'll be black.
 * We can override the generated thumbnail by providing this static thumbnail info instantiation, we can even use a function to generate the thumbnail on the
 * cpu as soon as this plugin is loaded into the application.
 */
static CFFGLThumbnailInfo ThumbnailInfo( THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, generateThumbnail() );
#else
static const Color thumbnail[] =
{
	Color( 0x000000FF ), Color( 0xFF0000FF ),
	Color( 0x00FF00FF ), Color( 0xFFFF00FF ),
};
static CFFGLThumbnailInfo ThumbnailInfo( 2, 2, thumbnail );
#endif

static const char vertexShaderCode[] = R"(#version 410
layout( location = 0 ) in vec4 vPosition;

void main()
{
	gl_Position = vPosition;
}
)";

static const char fragmentShaderCode[] = R"(#version 410
uniform float Intensity;

out vec4 fragColor;

float rand( vec2 n )
{
	return fract( sin( dot( n, vec2( 12.9898, 4.1414 ) ) ) * 43758.5453 );
}
float noise( vec2 p )
{
	vec2 ip = floor( p );
	vec2 u = fract( p );
	u = u * u * (3.0 - 2.0 * u);
	
	float res = mix(
		mix( rand( ip ),rand( ip + vec2( 1.0, 0.0 ) ), u.x ),
		mix( rand( ip + vec2( 0.0, 1.0 ) ), rand( ip + vec2( 1.0, 1.0 ) ), u.x ),
		u.y );
	return res * res;
}

void main()
{
	fragColor = vec4( vec3( noise( gl_FragCoord.xy ) ) * Intensity, 1.0 );
}
)";

CustomThumbnail::CustomThumbnail() :
	intensityLocation( -1 ),
	intensity( 0.0f )
{
	//Input properties
	SetMinInputs( 0 );
	SetMaxInputs( 0 );

	//Parameters
	SetParamInfof( PID_INTENSITY, "Intensity", FF_TYPE_STANDARD );
}
FFResult CustomThumbnail::InitGL( const FFGLViewportStruct* vp )
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
	intensityLocation  = shader.FindUniform( "Intensity" );

	//Use base-class init as success result so that it retains the viewport.
	return CFreeFrameGLPlugin::InitGL( vp );
}
FFResult CustomThumbnail::ProcessOpenGL( ProcessOpenGLStruct* pGL )
{
	//FFGL requires us to leave the context in a default state on return, so use this scoped binding to help us do that.
	ScopedShaderBinding shaderBinding( shader.GetGLID() );
	glUniform1f( intensityLocation, intensity );

	quad.Draw();

	return FF_SUCCESS;
}
FFResult CustomThumbnail::DeInitGL()
{
	shader.FreeGLResources();
	quad.Release();
	intensityLocation = -1;

	return FF_SUCCESS;
}

FFResult CustomThumbnail::SetFloatParameter( unsigned int dwIndex, float value )
{
	switch( dwIndex )
	{
	case PID_INTENSITY:
		intensity = value;
		break;

	default:
		return FF_FAIL;
	}

	return FF_SUCCESS;
}

float CustomThumbnail::GetFloatParameter( unsigned int index )
{
	switch( index )
	{
	case PID_INTENSITY:
		return intensity;
	}

	return 0.0f;
}
