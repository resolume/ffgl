#include "CustomThumbnail.h"
#include <math.h>//floor
#include "Thumb.h"
#include <png.h>
//#include <pngconf.h>
#include <pngstruct.h>
#include <pnginfo.h>
using namespace ffglex;

namespace PNGHelpers
{
struct Stream
{
	const void* start;
	size_t readPos = 0;
};

static void __cdecl readCallback( png_structp png, png_bytep data, png_size_t length )
{
	Stream& stream = *reinterpret_cast< Stream* >( png_get_io_ptr( png ) );
	memcpy( data, static_cast< const unsigned char* >( stream.start ) + stream.readPos, length );
	stream.readPos += length;
}

struct PNGErrorStruct
{
};

static void __cdecl errorCallback( png_structp png_ptr, png_const_charp error_message )
{
	throw PNGErrorStruct();
}
static void __cdecl warningCallback( png_structp, png_const_charp )
{
}

}//End namespace PNGHelpers

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
	/*
	for( Color& color : thumbnail )
	{
		unsigned char intensity = rand() % 256;
		color = Color( intensity, intensity, intensity, 255 );
	}
	*/

	PNGHelpers::Stream stream;
	stream.start = THUMBNAIL;

	png_structp pngReadStruct = png_create_read_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 );
	if( pngReadStruct == nullptr )
		return thumbnail;

	try
	{
		png_infop pngInfoStruct = png_create_info_struct( pngReadStruct );
		if( pngInfoStruct == nullptr )
		{
			png_destroy_read_struct( &pngReadStruct, 0, 0 );
			return thumbnail;
		}

		png_set_error_fn( pngReadStruct, 0, PNGHelpers::errorCallback, PNGHelpers::warningCallback );

		// read the header..
		png_set_read_fn( pngReadStruct, &stream, PNGHelpers::readCallback );

		png_read_info( pngReadStruct, pngInfoStruct );

		png_uint_32 width = 0, height = 0;
		int bitDepth = 0, colorType = 0, interlaceType = 0;
		// read the data from the header
		png_get_IHDR( pngReadStruct, pngInfoStruct, &width, &height, &bitDepth, &colorType, &interlaceType, 0, 0 );

		if( width != THUMBNAIL_WIDTH )
			return thumbnail;
		if( height != THUMBNAIL_HEIGHT )
			return thumbnail;

		// Expand to a minimum of 8 bits per channel if colour palette.
		if( colorType == PNG_COLOR_TYPE_PALETTE )
		{
			png_set_expand( pngReadStruct );

			// PALETTE mode is always without Alpha channel so we need to add this as well like with normal RGB
			png_set_add_alpha( pngReadStruct, 0xffffffff, PNG_FILLER_AFTER );
		}

		if( colorType == PNG_COLOR_TYPE_GRAY_ALPHA )
		{
			//expand to RGBA
			png_set_gray_to_rgb( pngReadStruct );
		}
		
		// Do the same if the colour type is not a palette but the bitdepth is < 8 per channel.
		if( bitDepth < 8 )
			png_set_expand( pngReadStruct );


		// Not sure why the validity of transparancy data should expand bitdepth, but i'll just leave it here.
		if( png_get_valid( pngReadStruct, pngInfoStruct, PNG_INFO_tRNS ) )
			png_set_expand( pngReadStruct );

		// If the format is RGB then add alpha channel and set it to fully opaque even when reading 32bit formats.
		if( colorType == PNG_COLOR_TYPE_RGB )
			png_set_add_alpha( pngReadStruct, 0xffffffff, PNG_FILLER_AFTER );

		// update the info in case something changed.
		png_read_update_info( pngReadStruct, pngInfoStruct );

		// get the new info.
		png_get_IHDR( pngReadStruct, pngInfoStruct, &width, &height, &bitDepth, &colorType, &interlaceType, 0, 0 );

		bool hasAlphaChan = (colorType & PNG_COLOR_MASK_ALPHA) != 0 || pngInfoStruct->num_trans > 0;

		int pixelDepth = pngInfoStruct->pixel_depth;
		size_t lineStride = width * (pixelDepth / 8); // pixelDepth is in bits convert to bytes

		//set output vars

		/**
			* In resapi bitDepth usually means bytesPerPixel. Not always though, but when decoding a png to Pixels object using the PixelsUtils
			* the Pixels object assumes it to be bytes per pixel. So to make sure that doesn't crash we output something other than bitDepth
			* through the bitDepth argument.
			*/
		//*bitDepth_ = pixelDepth / 8;
		//*lineStride_ = lineStride;
		//*width_ = width;
		//*height_ = height;
		//*hasAlpha_ = hasAlphaChan;

		/* swap bytes of 16-bit files to most significant byte first */
		if( bitDepth == 16 )
			png_set_swap( pngReadStruct );

		/* Load the image into a temp buffer in the pnglib format..
		*/
		std::vector< unsigned char > tempBuffer( height * lineStride );
		std::vector< png_bytep > rows( height );

		for( size_t y = 0; y < height; ++y )
			rows[ y ] = tempBuffer.data() + lineStride * y;

		try
		{
			png_read_image( pngReadStruct, rows.data() );
			png_read_end( pngReadStruct, pngInfoStruct );
		}
		catch( PNGHelpers::PNGErrorStruct& )
		{
			png_destroy_read_struct( &pngReadStruct, &pngInfoStruct, 0 );
			return thumbnail;
		}

		png_destroy_read_struct( &pngReadStruct, &pngInfoStruct, 0 );

		/* resize the passed in buffer if needed */

		//copy row by row
		for( int y = 0; y < (int)height; ++y )
		{
			const void* rowSrc = rows[ y ];
			void* rowDst = &thumbnail[ y * width ];
			memcpy( rowDst, rowSrc, lineStride );
		}
		for( Color& color : thumbnail )
		{
			unsigned char* channels = reinterpret_cast< unsigned char* >( &color.rgba );
			std::swap( channels[ 0 ], channels[ 3 ] );
			std::swap( channels[ 1 ], channels[ 2 ] );
		}
	}
	catch( PNGHelpers::PNGErrorStruct& )
	{
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
