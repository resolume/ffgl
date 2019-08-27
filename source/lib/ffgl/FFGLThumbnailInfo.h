////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FFGL.h
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

union Color
{
	struct Channels
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	} channels;
	FFUInt32 rgba;
};
class CFFGLThumbnailInfo
{
public:
	static CFFGLThumbnailInfo* GetInstance();

	CFFGLThumbnailInfo( FFUInt32 width, FFUInt32 height, std::vector< Color > pixels );

	FFUInt32 GetWidth() const;
	FFUInt32 GetHeight() const;
	const Color* GetPixels() const;

private:
	FFUInt32 width;
	FFUInt32 height;
	std::vector< Color > pixels;
};

#endif
