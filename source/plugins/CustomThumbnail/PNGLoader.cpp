#include "PNGLoader.h"
#include <png.h>
//#include <pngconf.h>
#include <pngstruct.h>
#include <pnginfo.h>

class ByteStream
{
public:
	ByteStream( const void* byteBuffer ) :
		bufferStart( byteBuffer ),
		readPos( 0 )
	{
	}

	void Read( void* dst, size_t size )
	{
		memcpy( dst, static_cast< const unsigned char* >( bufferStart ) + readPos, size );
		readPos += size;
	}

private:
	const void* bufferStart;
	size_t readPos;
};

namespace PNGHelpers
{
static void __cdecl readCallback( png_structp png, png_bytep data, png_size_t length )
{
	ByteStream& stream = *reinterpret_cast< ByteStream* >( png_get_io_ptr( png ) );
	stream.Read( data, length );
}

static void __cdecl errorCallback( png_structp png_ptr, png_const_charp error_message )
{
	throw std::runtime_error( error_message );
}
static void __cdecl warningCallback( png_structp, png_const_charp )
{
}

}//End namespace PNGHelpers

FFUInt32 PNGLoader::ParsePNGWidth( const void* pngData )
{
	png_structp pngReadStruct = png_create_read_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 );
	if( pngReadStruct == nullptr )
		return 0;
	png_infop pngInfoStruct = png_create_info_struct( pngReadStruct );
	if( pngInfoStruct == nullptr )
	{
		png_destroy_read_struct( &pngReadStruct, 0, 0 );
		return 0;
	}

	//Set up the read function so that we can read the png from the provided pngData.
	ByteStream stream( pngData );
	png_set_error_fn( pngReadStruct, 0, PNGHelpers::errorCallback, PNGHelpers::warningCallback );
	png_set_read_fn( pngReadStruct, &stream, PNGHelpers::readCallback );

	try
	{
		//Fill the pngInfoStruct with information from the png buffer.
		png_read_info( pngReadStruct, pngInfoStruct );

		png_uint_32 width = 0, height = 0;
		int bitDepth = 0, colorType = 0, interlaceType = 0;
		//Get the header information from the pngInfo structure.
		png_get_IHDR( pngReadStruct, pngInfoStruct, &width, &height, &bitDepth, &colorType, &interlaceType, 0, 0 );

		return width;
	}
	catch( ... )
	{
	}

	return 0;
}
FFUInt32 PNGLoader::ParsePNGHeight( const void* pngData )
{
	png_structp pngReadStruct = png_create_read_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 );
	if( pngReadStruct == nullptr )
		return 0;
	png_infop pngInfoStruct = png_create_info_struct( pngReadStruct );
	if( pngInfoStruct == nullptr )
	{
		png_destroy_read_struct( &pngReadStruct, 0, 0 );
		return 0;
	}

	//Set up the read function so that we can read the png from the provided pngData.
	ByteStream stream( pngData );
	png_set_error_fn( pngReadStruct, 0, PNGHelpers::errorCallback, PNGHelpers::warningCallback );
	png_set_read_fn( pngReadStruct, &stream, PNGHelpers::readCallback );

	try
	{
		//Fill the pngInfoStruct with information from the png buffer.
		png_read_info( pngReadStruct, pngInfoStruct );

		png_uint_32 width = 0, height = 0;
		int bitDepth = 0, colorType = 0, interlaceType = 0;
		//Get the header information from the pngInfo structure.
		png_get_IHDR( pngReadStruct, pngInfoStruct, &width, &height, &bitDepth, &colorType, &interlaceType, 0, 0 );

		return height;
	}
	catch( ... )
	{
	}

	return 0;
}
std::vector< CFFGLColor > PNGLoader::ParsePNGPixels( const void* pngData )
{
	std::vector< CFFGLColor > pixels;

	png_structp pngReadStruct = png_create_read_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 );
	if( pngReadStruct == nullptr )
		return pixels;
	png_infop pngInfoStruct = png_create_info_struct( pngReadStruct );
	if( pngInfoStruct == nullptr )
	{
		png_destroy_read_struct( &pngReadStruct, 0, 0 );
		return pixels;
	}

	//Set up the read function so that we can read the png from the provided pngData.
	ByteStream stream( pngData );
	png_set_error_fn( pngReadStruct, 0, PNGHelpers::errorCallback, PNGHelpers::warningCallback );
	png_set_read_fn( pngReadStruct, &stream, PNGHelpers::readCallback );

	try
	{
		//Fill the pngInfoStruct with information from the png buffer.
		png_read_info( pngReadStruct, pngInfoStruct );

		png_uint_32 width = 0, height = 0;
		int bitDepth = 0, colorType = 0, interlaceType = 0;
		//Get the header information from the pngInfo structure.
		png_get_IHDR( pngReadStruct, pngInfoStruct, &width, &height, &bitDepth, &colorType, &interlaceType, 0, 0 );

		pixels.resize( width * height );

		switch( colorType )
		{
		case PNG_COLOR_TYPE_GRAY:                // (bit depths 1, 2, 4, 8, 16)
			png_set_expand( pngReadStruct );     //Expand the grayscale to at least a bitdepth of 8.
			png_set_gray_to_rgb( pngReadStruct );//Converts the grayscale from 1 channel to RGB.
			break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:          // (bit depths 8, 16)
			png_set_gray_to_rgb( pngReadStruct );// //Converts the grayscale from 1 channel to RGB.
			//Already contains alpha \o/
			break;
		case PNG_COLOR_TYPE_PALETTE:                                         // (bit depths 1, 2, 4, 8)
			png_set_expand( pngReadStruct );                                 //Turns the palette image into RGB
			png_set_add_alpha( pngReadStruct, 0xffffffff, PNG_FILLER_AFTER );//Palettes dont contain alpha, so always add it.
			break;
		case PNG_COLOR_TYPE_RGB:                                             // (bit_depths 8, 16)
			png_set_add_alpha( pngReadStruct, 0xffffffff, PNG_FILLER_AFTER );//There's no alpha channel so add it as we need to end up with RGBA.
			break;
		case PNG_COLOR_TYPE_RGB_ALPHA:// (bit_depths 8, 16)
			//PERFECT! This is what we want.
			break;
		case PNG_COLOR_MASK_PALETTE:
			break;
			//case PNG_COLOR_MASK_COLOR:
			//break;
			//case PNG_COLOR_MASK_ALPHA:
			//break;
		}

		//tRNS chunks aren't actually a full alpha channel so if it's present we need to enable expansion aswell so that
		//the chunk is converted into a full alpha channel.
		if( png_get_valid( pngReadStruct, pngInfoStruct, PNG_INFO_tRNS ) )
			png_set_expand( pngReadStruct );

		//We're not supporting 16bit color channels so convert these to 8bit.
		png_set_scale_16( pngReadStruct );
		//png_set_bgr

		//Reinitialize the info struct and our header information values so that they'll all be updated to what we've requested libPNG to do during import.
		png_read_update_info( pngReadStruct, pngInfoStruct );
		png_get_IHDR( pngReadStruct, pngInfoStruct, &width, &height, &bitDepth, &colorType, &interlaceType, 0, 0 );

		//PNGs are stored in network byte order (big-endian). We're running on a little endian system so swap the byte order.
		//if( bitDepth == 16 )
		//png_set_swap( pngReadStruct );

		size_t lineStride = width * ( pngInfoStruct->pixel_depth / 8 );// pixelDepth is in bits convert to bytes

		//Load the image into a temporary buffer so that if anything goes wrong while reading we dont risk
		//outputting garbage through the output buffer due to a partial read.
		std::vector< unsigned char > tempBuffer( height * lineStride );
		std::vector< png_bytep > rows( height );
		for( size_t y = 0; y < height; ++y )
			rows[ y ] = tempBuffer.data() + y * lineStride;

		try
		{
			png_read_image( pngReadStruct, rows.data() );
			png_read_end( pngReadStruct, pngInfoStruct );
		}
		catch( std::exception& )
		{
			png_destroy_read_struct( &pngReadStruct, &pngInfoStruct, 0 );
			return pixels;
		}
		//We no longer need the png headers as we're done reading the data from it, release them here so that we cant forget.
		png_destroy_read_struct( &pngReadStruct, &pngInfoStruct, 0 );

		//The tempBuffer is exactly the required size as we've enforced RGBA8888 as libPNG's output format.
		memcpy( pixels.data(), tempBuffer.data(), tempBuffer.size() );

		for( CFFGLColor& color : pixels )
		{
			//libPNG outputs ABGR which is the big-endian format of our RGBA output so we need to flip the bytes.
			unsigned char* channels = reinterpret_cast< unsigned char* >( &color.rgba );
			std::swap( channels[ 0 ], channels[ 3 ] );
			std::swap( channels[ 1 ], channels[ 2 ] );
		}
	}
	catch( std::exception& )
	{
	}

	return pixels;
}