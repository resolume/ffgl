#pragma once
#include <vector>
#include <FFGLSDK.h>

class PNGLoader
{
public:
	static FFUInt32 ParsePNGWidth( const void* pngData );
	static FFUInt32 ParsePNGHeight( const void* pngData );
	static std::vector< CFFGLColor > ParsePNGPixels( const void* pngData );
};