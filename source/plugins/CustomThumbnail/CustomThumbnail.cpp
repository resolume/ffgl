#include "CustomThumbnail.h"
#include <math.h>//floor
using namespace ffglex;

enum ParamID
{
	PID_INTENSITY
};

static CFFGLPluginInfo PluginInfo(
	PluginFactory< CustomThumbnail >,          // Creation method
	"RS03",                                    // Plugin unique ID
	"Custom Thumbnail Example",                // Plugin name
	2,                                         // API major version number
	1,                                         // API minor version number
	1,                                         // Plugin major version number
	000,                                       // Plugin minor version number
	FF_SOURCE,                                 // Plugin type
	"Sample FFGL plugin with custom thumbnail",// Plugin description
	"Resolume FFGL Example"                    // About
);

#define THUMBNAIL_METHOD_EMBEDDED_RAW 1
#define THUMBNAIL_METHOD_EMBEDDED_PNG 2
#define THUMBNAIL_METHOD_CPU_GENERATED 3
/**
 * To provide thumbnail data to the host we need the thumbnail's dimensions and the rgba data. We could come up multiple with ways to provide these but
 * this example shows the three most common scenarios. To see any of these scenarios you have to changethe value of this define to one of the values above.
 * The RAW method shows how you would specify the thumbnail if you've got raw RGBA data embedded in your plugin. The PNG method shows how you could specify the
 * thumbnail data if you've embedded some encoded image (like png). Last is the CPU example that shows you can also generate a thumbnail on the cpu should you want to.
 */
#define THUMBNAILMETHOD THUMBNAIL_METHOD_EMBEDDED_PNG

#if THUMBNAILMETHOD == THUMBNAIL_METHOD_EMBEDDED_RAW
static const CFFGLColor thumbnail[] = {
	CFFGLColor( 0x000000FF ),
	CFFGLColor( 0xFF0000FF ),
	CFFGLColor( 0x00FF00FF ),
	CFFGLColor( 0xFFFF00FF ),
};
static CFFGLThumbnailInfo ThumbnailInfo( 2, 2, thumbnail );
#elif THUMBNAILMETHOD == THUMBNAIL_METHOD_EMBEDDED_PNG
#include "PNGLoader.h"
#include "Thumb.h"
//The PNG may be of any dimensions so we use the PNGLoader to get these dimensions from the png data.
static CFFGLThumbnailInfo ThumbnailInfo( PNGLoader::ParsePNGWidth( THUMBNAIL ), PNGLoader::ParsePNGHeight( THUMBNAIL ), PNGLoader::ParsePNGPixels( THUMBNAIL ) );
#elif THUMBNAILMETHOD == THUMBNAIL_METHOD_CPU_GENERATED
/**
 * The plugin gets to decide how large it's thumbnail is, it's then up to the host to determine how to represent this thumbnail
 * on the screen. Some hosts may crop the thumbnails, some may stretch it, some apply autosizing to fit the thumbnail.
 * Resolume uses 160x120 thumbnails, and since we're testing with Resolume being the host we're matching this resolution so that
 * we're getting a pixel perfect thumbnail.
 */
static const FFUInt32 THUMBNAIL_WIDTH  = 160;
static const FFUInt32 THUMBNAIL_HEIGHT = 120;
std::vector< CFFGLColor > generateThumbnail()
{
	/**
	 * NOTE: There's no active context here and the plugin has not been initialized. This function can only be used to generate a thumbnail on the CPU.
	 * You may want to use a function like this one, if the thumbnail you've embedded has some encoding applied. You can use this function to decode, eg
	 * decode from png to raw RGBA.
	 */
	std::vector< CFFGLColor > thumbnail( THUMBNAIL_WIDTH * THUMBNAIL_HEIGHT );

	for( CFFGLColor& color : thumbnail )
	{
		unsigned char intensity = rand() % 256;
		color                   = CFFGLColor( intensity, intensity, intensity, 255 );
	}

	return thumbnail;
}
/**
 * The default thumbnail rendered by the host is kind of boring as we've set our intensity to 0 by default so it'll be black.
 * We can override the generated thumbnail by providing this static thumbnail info instantiation, we can even use a function to generate the thumbnail on the
 * cpu as soon as this plugin is loaded into the application.
 */
static CFFGLThumbnailInfo ThumbnailInfo( THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, generateThumbnail() );
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

	FFGLLog::LogToHost( "Created CustomThumbnail generator" );
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
	intensityLocation = shader.FindUniform( "Intensity" );

	//Use base-class init as success result so that it retains the viewport.
	return CFFGLPlugin::InitGL( vp );
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
