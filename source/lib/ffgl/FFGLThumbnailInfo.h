////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FFGLThumbnailInfo.h
//
// FreeFrame is an open-source cross-platform real-time video effects plugin system.
// It provides a framework for developing video effects plugins and hosts on Windows,
// Linux and Mac OSX.
//
// Copyright (c) 2018 www.freeframe.org
// All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Redistribution and use in source and binary forms, with or without modification,
//	are permitted provided that the following conditions are met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materials provided with the
//    distribution.
//  * Neither the name of FreeFrame nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//
//	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//	IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
//	INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
//	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
//	OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
//	OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
//	OF THE POSSIBILITY OF SUCH DAMAGE.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// First version, Menno Vink (menno@resolume.com)
// www.resolume.com
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FFGLTHUMBNAILINFO_STANDARD
#define FFGLTHUMBNAILINFO_STANDARD
#include <vector>
#include "FFGL.h"

struct CFFGLColor
{
	/// Creates a new Color that's automatically initialized to transparent black.
	CFFGLColor();
	/// Creates a new color from an unsigned integer that contains the data for all the channels.
	/// The unsigned integer needs to be in the 0xRRGGBBAA format. This means that the Red channel
	/// needs to be the most significant byte, then green, then blue and alpha is the least significant byte.
	CFFGLColor( FFUInt32 rgba );
	/// Creates a color where you can specify the color channel's values seperately without having to manually
	/// handle the order of the RGBA channels.
	CFFGLColor( unsigned char r, unsigned char g, unsigned char b, unsigned char a );

	FFUInt32 rgba;//!< 1 byte for each channel. Red is MSB while Alpha is LSB. (0xRRGGBBAA)
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class		CFFGLThumbnailInfo
/// \brief		CFFGLThumbnailInfo manages static information concerning a plugin's thumbnail.
/// \author		Menno Vink (menno@resolume.com)
/// \date		2019-08-29
///
/// The CFFGLThumbnailInfo class manages static information related to a FreeFrameGL plugin's thumbnail.
/// If you want to expose an embedded thumbnail inside your plugin you should declare a static instance of this class.
/// Then when a host that supports embedded thumbnails asks the plugin for it's thumbnail the static instance
/// is accessed and the embedded plugin is  returned to the host.
/// As far as thumbnail size goes, it's the plugin that gets to decide what size of thumbnail it wants to provide.
/// When a host gets a thumbnail at a size different than the thumbnail size that it wants it's up to the host to determine how to go from the
/// plugin's thumbnail's size to the size of the thumbnail that the host wants to show.
/// Thumbnails have their origin as top-left. So the first pixel color is the most top left pixel, the second pixel is the next pixel on the row, so a one pixel to the right.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CFFGLThumbnailInfo
{
public:
	static CFFGLThumbnailInfo* GetInstance();

	/// This constructor can be used when you're generating a vector of colors over which you want to move ownership over
	/// into this thumbnail info instance. This may be usefull when you have a function to generate the thumbnail because you
	/// can use the function's return value to provide the thumbnail's colors.
	///
	/// \param width		The width of the thumbnail in number of pixels.
	/// \param height		The height of the thumbnail in number of pixels.
	/// \param ownedPixels	A vector of colors representing the thumbnail pixels' colors. This vector is expected to be width*height in size.
	CFFGLThumbnailInfo( FFUInt32 width, FFUInt32 height, std::vector< CFFGLColor > ownedPixels );
	/// This constructor can be used when you've got a static array of color values representing the thumbnail's colors.
	/// This may be usefull when you've got a plain array of rgba colors that you want to use as thumbnail without having to copy their ownership
	/// over into this thumbnail info instance.
	///
	/// \param width		The width of the thumbnail in number of pixels.
	/// \param height		The height of the thumbnail in number of pixels.
	/// \param ownedPixels	A vector of colors representing the thumbnail pixels' colors. This vector is expected to be width*height in size.
	CFFGLThumbnailInfo( FFUInt32 width, FFUInt32 height, const CFFGLColor* pixelData );

	/// Get the width of the thumbnail in number of pixels.
	FFUInt32 GetWidth() const;
	/// Get the height of the thumbnail in number of pixels.
	FFUInt32 GetHeight() const;
	/// Gets the thumbnail's pixel colors. This is a contiguous array of colors at the size of width * height;
	const CFFGLColor* GetPixels() const;

private:
	FFUInt32 width;                       //!< Width of the thumbnail in number of pixels.
	FFUInt32 height;                      //!< Height of the thumbnail in number of pixels.
	std::vector< CFFGLColor > ownedPixels;//!< Array of thumbnail data owned by this instance.
	const CFFGLColor* pixelData;          //!< A pointer to the array of thumbnail pixel data. The thumbnail's colors will be read from this array.
};

#endif
