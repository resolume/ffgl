#include "FFGLThumbnailInfo.h"
//#include <assert.h>
//#include <stdlib.h>
//#include <memory.h>

namespace ffgl
{
static CFFGLThumbnailInfo* instance = nullptr;

CFFGLThumbnailInfo* CFFGLThumbnailInfo::GetInstance()
{
	return instance;
}

CFFGLThumbnailInfo::CFFGLThumbnailInfo( FFUInt32 width, FFUInt32 height, std::vector< Color > ownedPixels ) :
	width( width ), height( height ),
	ownedPixels( std::move( ownedPixels ) )
{
	this->ownedPixels.resize( width * height );
	//Technically the vector's data doesn't have to be in contiguous memory, but for all stl implementations we know it is so
	//we can just do this.
	pixelData = this->ownedPixels.data();
	instance = this;
}
CFFGLThumbnailInfo::CFFGLThumbnailInfo( FFUInt32 width, FFUInt32 height, const Color* pixelData ) :
	width( width ), height( height ),
	pixelData( pixelData )
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
const Color* CFFGLThumbnailInfo::GetPixels() const
{
	return pixelData;
}

}//End namespace ffgl