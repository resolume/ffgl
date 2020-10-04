#include "FFGLThumbnailInfo.h"
//#include <assert.h>
//#include <stdlib.h>
//#include <memory.h>

static CFFGLThumbnailInfo* instance = nullptr;

CFFGLColor::CFFGLColor() :
	rgba( 0 )
{
}
CFFGLColor::CFFGLColor( FFUInt32 rgba ) :
	rgba( rgba )
{
}
CFFGLColor::CFFGLColor( unsigned char r, unsigned char g, unsigned char b, unsigned char a ) :
	rgba( 0 )
{
	rgba |= FFUInt32( r ) << 24;
	rgba |= FFUInt32( g ) << 16;
	rgba |= FFUInt32( b ) << 8;
	rgba |= FFUInt32( a ) << 0;
}

CFFGLThumbnailInfo* CFFGLThumbnailInfo::GetInstance()
{
	return instance;
}

CFFGLThumbnailInfo::CFFGLThumbnailInfo( FFUInt32 width, FFUInt32 height, std::vector< CFFGLColor > ownedPixels ) :
	width( width ), height( height ), ownedPixels( std::move( ownedPixels ) )
{
	this->ownedPixels.resize( width * height );
	//Technically the vector's data doesn't have to be in contiguous memory, but for all stl implementations we know it is so
	//we can just do this.
	pixelData = this->ownedPixels.data();
	instance  = this;
}
CFFGLThumbnailInfo::CFFGLThumbnailInfo( FFUInt32 width, FFUInt32 height, const CFFGLColor* pixelData ) :
	width( width ), height( height ), pixelData( pixelData )
{
	instance = this;
}

FFUInt32 CFFGLThumbnailInfo::GetWidth() const
{
	return width;
}
FFUInt32 CFFGLThumbnailInfo::GetHeight() const
{
	return height;
}
const CFFGLColor* CFFGLThumbnailInfo::GetPixels() const
{
	return pixelData;
}
