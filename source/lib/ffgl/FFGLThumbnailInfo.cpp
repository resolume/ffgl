#include "FFGLThumbnailInfo.h"
//#include <assert.h>
//#include <stdlib.h>
//#include <memory.h>

static CFFGLThumbnailInfo* instance = nullptr;

CFFGLThumbnailInfo* CFFGLThumbnailInfo::GetInstance()
{
	return instance;
}

CFFGLThumbnailInfo::CFFGLThumbnailInfo( FFUInt32 width, FFUInt32 height, std::vector< Color > pixels ) :
	width( width ), height( height ),
	pixels( std::move( pixels ) )
{
	this->pixels.resize( width * height );
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
	//Technically the vector's data doesn't have to be in contiguous memory, but for all stl implementations we know it is so
	//we can just do this.
	return pixels.data();
}