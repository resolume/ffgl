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
// First version, Marcus Clements (marcus@freeframe.org)
// www.freeframe.org
//
// FreeFrame 1.0 upgrade by Pete Warden
// www.petewarden.com
//
// FreeFrame 1.0 - 03 upgrade by Gualtiero Volpe
// Gualtiero.Volpe@poste.it
//
// #ifdef tweaks for FreeFrameGL upgrade by Trey Harrison
// www.harrisondigitalmedia.com
//
// FFGL 1.0 by Trey Harrison
// www.harrisondigitalmedia.com
//
// FFGL 2.0 by Menno Vink (menno@resolume.com)
// www.resolume.com
// -Removed the old FreeFrame functionality, and thus CPU processing is no longer supported.
// -Completed the FF_TYPE_OPTION parameter type. Plugins can add parameter elements which hosts
//  should use to show a dropdown styled parameter. When one of the options is selected the parameter
//  will get a value equal to that of the option's value.
// -Completed the FF_TYPE_BUFFER parameter type. When this parameter is used it should provide a usage indication
//  which the host can then use to fill that buffer with the requested data. This can be used by plugins to
//  access the host's global fft data for example.
//
// FFGL 2.1 by Menno Vink (menno@resolume.com)
// www.resolume.com
// -Added support for embedded thumbnails.
// -Reintroduction of RAII bindings to help protect the host's context state.
// -Added file parameters
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FFGL2_H__
#define __FFGL2_H__

//////////////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////////////

//include the appropriate OpenGL headers for the compiler

#if defined( _WIN32 )
#define WIN32_LEAN_AND_MEAN//Exclude rarely-used stuff from Windows headers
#define _WINSOCKAPI_       //Prevent inclusion of winsock
//Defines to prevent windows.h from making all kinds of defines which may conflict with actual user code.
#define NOGDICAPMASKS
#define NOVIRTUALKEYCODES
#define NOWINMESSAGES
#define NOWINSTYLES
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOSHOWWINDOW
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
//#define NOGDI        //We need wingdi because it defines wglGetProcAddress
#define NOKERNEL
#define NOUSER
#define NONLS
#define NOMB
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOMSG
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOWINOFFSETS
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
//Defines to prevent WinUser.h from making all kinds of defines which may conflict with actual user code.
#define NOAPISET
#define NOUSER
#define NORESOURCE
#define NOSCROLL
#define NOSHOWWINDOW
#define NOVIRTUALKEYCODES
#define NOWH
#define NODESKTOP
#define NOWINDOWSTATION
#define NOSECURITY
#define NOMSG
#define NOWINOFFSETS
#define NOWINMESSAGES
#define NONCMESSAGES
#define NOKEYSTATES
#define NOTRACKMOUSEEVENT
#define NOWINSTYLES
#define NOCLIPBOARD
#define NOMSG
#define NODEFERWINDOWPOS
#define NOCTLMGR
#define NOMSG
#define NOCLIPBOARD
#define NOMSG
#define NOSYSMETRICS
#define NOMENUS
#define NODRAWTEXT
#define NOSCROLL
#define NOMB
#define NOCOLOR
#define NOWINOFFSETS
#define NOWH
#define NOMENUS
#define NOSYSCOMMANDS
#define NOICONS
#define NOCTLMGR
#define NOWINSTYLES
#define NOWINMESSAGES
#define NOWINMESSAGES
#define NOMSG
#define NOWINMESSAGES
#define NOWINSTYLES
#define NOWINSTYLES
#define NOWINMESSAGES
#define NOWINSTYLES
#define NOWINMESSAGES
#define NOMDI
#define NOMSG
#define NOHELP
#define NOSYSPARAMSINFO
#define NOWINABLE
#define NO_STATE_FLAGS
#include <windows.h>
#include "../glsdk_0_5_2/glload/include/gl_4_1.h"
#undef NOGDICAPMASKS
#undef NOVIRTUALKEYCODES
#undef NOWINMESSAGES
#undef NOWINSTYLES
#undef NOSYSMETRICS
#undef NOMENUS
#undef NOICONS
#undef NOKEYSTATES
#undef NOSYSCOMMANDS
#undef NORASTEROPS
#undef NOSHOWWINDOW
#undef OEMRESOURCE
#undef NOATOM
#undef NOCLIPBOARD
#undef NOCOLOR
#undef NOCTLMGR
#undef NODRAWTEXT
//#undef NOGDI
#undef NOKERNEL
#undef NOUSER
#undef NONLS
#undef NOMB
#undef NOMEMMGR
#undef NOMETAFILE
#undef NOMINMAX
#undef NOMSG
#undef NOOPENFILE
#undef NOSCROLL
#undef NOSERVICE
#undef NOSOUND
#undef NOTEXTMETRIC
#undef NOWH
#undef NOWINOFFSETS
#undef NOCOMM
#undef NOKANJI
#undef NOHELP
#undef NOPROFILER
#undef NODEFERWINDOWPOS
#undef NOMCX
#undef NOAPISET
#undef NOUSER
#undef NORESOURCE
#undef NOSCROLL
#undef NOSHOWWINDOW
#undef NOVIRTUALKEYCODES
#undef NOWH
#undef NODESKTOP
#undef NOWINDOWSTATION
#undef NOSECURITY
#undef NOMSG
#undef NOWINOFFSETS
#undef NOWINMESSAGES
#undef NONCMESSAGES
#undef NOKEYSTATES
#undef NOTRACKMOUSEEVENT
#undef NOWINSTYLES
#undef NOCLIPBOARD
#undef NOMSG
#undef NODEFERWINDOWPOS
#undef NOCTLMGR
#undef NOMSG
#undef NOCLIPBOARD
#undef NOMSG
#undef NOSYSMETRICS
#undef NOMENUS
#undef NODRAWTEXT
#undef NOSCROLL
#undef NOMB
#undef NOCOLOR
#undef NOWINOFFSETS
#undef NOWH
#undef NOMENUS
#undef NOSYSCOMMANDS
#undef NOICONS
#undef NOCTLMGR
#undef NOWINSTYLES
#undef NOWINMESSAGES
#undef NOWINMESSAGES
#undef NOMSG
#undef NOWINMESSAGES
#undef NOWINSTYLES
#undef NOWINSTYLES
#undef NOWINMESSAGES
#undef NOWINSTYLES
#undef NOWINMESSAGES
#undef NOMDI
#undef NOMSG
#undef NOHELP
#undef NOSYSPARAMSINFO
#undef NOWINABLE
#undef NO_STATE_FLAGS
typedef unsigned __int16 FFUInt16;
typedef unsigned __int32 FFUInt32;
typedef unsigned __int64 FFUInt64;
#else
#if defined( TARGET_OS_MAC )
#include <OpenGL/gl3.h>
#elif defined( __linux__ )
#include <GL/gl.h>
#else
#error define this for your OS
#endif

extern "C" {
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint16_t FFUInt16;
typedef uint32_t FFUInt32;
typedef uint64_t FFUInt64;
#endif

// Function codes
static const FFUInt32 FF_GET_INFO                          = 0;
static const FFUInt32 FF_INITIALISE_V2                     = 34;
static const FFUInt32 FF_DEINITIALISE                      = 2;
static const FFUInt32 FF_GET_NUM_PARAMETERS                = 4;
static const FFUInt32 FF_GET_PARAMETER_NAME                = 5;
static const FFUInt32 FF_GET_PARAMETER_DEFAULT             = 6;
static const FFUInt32 FF_GET_PARAMETER_DISPLAY             = 7;
static const FFUInt32 FF_SET_PARAMETER                     = 8;
static const FFUInt32 FF_GET_PARAMETER                     = 9;
static const FFUInt32 FF_GET_PLUGIN_CAPS                   = 10;
static const FFUInt32 FF_ENABLE_PLUGIN_CAP                 = 49;
static const FFUInt32 FF_GET_EXTENDED_INFO                 = 13;
static const FFUInt32 FF_GET_PARAMETER_TYPE                = 15;
static const FFUInt32 FF_GET_INPUT_STATUS                  = 16;
static const FFUInt32 FF_PROCESS_OPENGL                    = 17;
static const FFUInt32 FF_INSTANTIATE_GL                    = 18;
static const FFUInt32 FF_DEINSTANTIATE_GL                  = 19;
static const FFUInt32 FF_SET_TIME                          = 20;
static const FFUInt32 FF_CONNECT                           = 21;
static const FFUInt32 FF_DISCONNECT                        = 22;
static const FFUInt32 FF_RESIZE                            = 23;
static const FFUInt32 FF_GET_NUM_PARAMETER_ELEMENTS        = 31;
static const FFUInt32 FF_GET_PARAMETER_ELEMENT_NAME        = 35;
static const FFUInt32 FF_GET_PARAMETER_ELEMENT_DEFAULT     = 36;
static const FFUInt32 FF_SET_PARAMETER_ELEMENT_VALUE       = 37;
static const FFUInt32 FF_GET_PARAMETER_USAGE               = 32;
static const FFUInt32 FF_GET_PLUGIN_SHORT_NAME             = 33;
static const FFUInt32 FF_SET_BEATINFO                      = 38;
static const FFUInt32 FF_SET_HOSTINFO                      = 39;
static const FFUInt32 FF_SET_SAMPLERATE                    = 40;
static const FFUInt32 FF_GET_RANGE                         = 41;
static const FFUInt32 FF_GET_THUMBNAIL                     = 42;
static const FFUInt32 FF_GET_NUM_FILE_PARAMETER_EXTENSIONS = 43;
static const FFUInt32 FF_GET_FILE_PARAMETER_EXTENSION      = 44;
static const FFUInt32 FF_GET_PRAMETER_VISIBILITY           = 45;
static const FFUInt32 FF_GET_PARAMETER_EVENTS              = 46;
static const FFUInt32 FF_GET_NUM_ELEMENT_SEPARATORS        = 47;
static const FFUInt32 FF_GET_SEPARATOR_ELEMENT_INDEX       = 48;
//Next ID = 50

//Previously used function codes that are no longer in use. Should prevent using
//these numbers for new function codes.
//static const FFUInt32 FF_INITIALISE            = 1;
//static const FFUInt32 FF_PROCESSFRAME          = 3;
//static const FFUInt32 FF_INSTANTIATE           = 11;
//static const FFUInt32 FF_DEINSTANTIATE         = 12;
//static const FFUInt32 FF_PROCESSFRAMECOPY      = 14;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FreeFrame defines
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum
{
	FF_SUCCESS = 0,
	FF_FAIL    = 0xFFFFFFFF
};
typedef FFUInt32 FFResult;

// Return values
static const FFUInt32 FF_TRUE        = 1;
static const FFUInt32 FF_FALSE       = 0;
static const FFUInt32 FF_SUPPORTED   = 1;
static const FFUInt32 FF_UNSUPPORTED = 0;

// Plugin types
static const FFUInt32 FF_EFFECT = 0;
static const FFUInt32 FF_SOURCE = 1;
static const FFUInt32 FF_MIXER  = 2;

// Plugin capabilities
static const FFUInt32 FF_CAP_SET_TIME                     = 5;
static const FFUInt32 FF_CAP_MINIMUM_INPUT_FRAMES         = 10;
static const FFUInt32 FF_CAP_MAXIMUM_INPUT_FRAMES         = 11;
static const FFUInt32 FF_CAP_TOP_LEFT_TEXTURE_ORIENTATION = 16;
//Previously used capability codes that are no longer in use. New codes should prevent using
//these numbers for new capability codes.
//static const FFUInt32 FF_CAP_16BITVIDEO         = 0;
//static const FFUInt32 FF_CAP_24BITVIDEO         = 1;
//static const FFUInt32 FF_CAP_32BITVIDEO         = 2;
//static const FFUInt32 FF_CAP_PROCESSFRAMECOPY   = 3;
//static const FFUInt32 FF_CAP_PROCESSOPENGL      = 4;
//static const FFUInt32 FF_CAP_COPYORINPLACE      = 15;

// Old return values that were used for the FF_CAP_COPYORINPLACE capability.
//static const FFUInt32 FF_CAP_PREFER_NONE    = 0;
//static const FFUInt32 FF_CAP_PREFER_INPLACE = 1;
//static const FFUInt32 FF_CAP_PREFER_COPY    = 2;
//static const FFUInt32 FF_CAP_PREFER_BOTH    = 3;

// Parameter types
static const FFUInt32 FF_TYPE_BOOLEAN    = 0;
static const FFUInt32 FF_TYPE_EVENT      = 1;
static const FFUInt32 FF_TYPE_RED        = 2;
static const FFUInt32 FF_TYPE_GREEN      = 3;
static const FFUInt32 FF_TYPE_BLUE       = 4;
static const FFUInt32 FF_TYPE_XPOS       = 5;
static const FFUInt32 FF_TYPE_YPOS       = 6;
static const FFUInt32 FF_TYPE_STANDARD   = 10;
static const FFUInt32 FF_TYPE_OPTION     = 11;
static const FFUInt32 FF_TYPE_BUFFER     = 12;
static const FFUInt32 FF_TYPE_INTEGER    = 13;
static const FFUInt32 FF_TYPE_FILE       = 14;
static const FFUInt32 FF_TYPE_TEXT       = 100;
static const FFUInt32 FF_TYPE_HUE        = 200;
static const FFUInt32 FF_TYPE_SATURATION = 201;
static const FFUInt32 FF_TYPE_BRIGHTNESS = 202;
static const FFUInt32 FF_TYPE_ALPHA      = 203;

// Input status
static const FFUInt32 FF_INPUT_NOTINUSE = 0;
static const FFUInt32 FF_INPUT_INUSE    = 1;

// Parameter usages
static const FFUInt32 FF_USAGE_STANDARD = 0;
static const FFUInt32 FF_USAGE_FFT      = 1;

// Parameter events flags
static const FFUInt64 FF_EVENT_FLAG_VISIBILITY    = 0x01; //A parameter's visibility changed.
//Not supported yet, but possibly in the future we would like these events as well:
//static const FFUInt64 FF_EVENT_FLAG_VALUE         = 0x02; //A parameter's current value changed.
//static const FFUInt64 FF_EVENT_FLAG_DEFAULT_VALUE = 0x04; //A parameter's default value changed.
//static const FFUInt64 FF_EVENT_FLAG_NAME          = 0x08; //A parameter's name changed.
//static const FFUInt64 FF_EVENT_FLAG_RANGE         = 0x10; //A parameter's range has been changed.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FreeFrame Types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef union FFMixed
{
	FFUInt32 UIntValue;
	void* PointerValue;
} FFMixed;

typedef void* FFInstanceID;

// PluginInfoStruct
typedef struct PluginInfoStructTag
{
	FFUInt32 APIMajorVersion;
	FFUInt32 APIMinorVersion;
	char PluginUniqueID[ 4 ];// 4 chars uniqueID - not null terminated
	char PluginName[ 16 ];   // 16 chars plugin friendly name - not null terminated
	FFUInt32 PluginType;     // Effect or source
} PluginInfoStruct;

// PluginExtendedInfoStruct
typedef struct PluginExtendedInfoStructTag
{
	FFUInt32 PluginMajorVersion;
	FFUInt32 PluginMinorVersion;
	const char* Description;
	const char* About;
	FFUInt32 FreeFrameExtendedDataSize;
	void* FreeFrameExtendedDataBlock;
} PluginExtendedInfoStruct;

// SetParameterStruct
typedef struct SetParameterStructTag
{
	FFUInt32 ParameterNumber;
	FFMixed NewParameterValue;
} SetParameterStruct;

// SetBeatinfoStruct
typedef struct SetBeatinfoStructTag
{
	float bpm;
	float barPhase;
} SetBeatinfoStruct;

// SetHostinfoStruct
typedef struct SetHostinfoStructTag
{
	const char* name;
	const char* version;
} SetHostinfoStruct;

typedef struct RangeStructTag
{
	float min;
	float max;
} RangeStruct;

typedef struct GetRangeStructTag
{
	FFUInt32 parameterNumber;
	RangeStruct range;
} GetRangeStruct;

/**
 * 
 */
typedef struct GetThumbnailStructTag
{
	FFUInt32 width;           //!< Used as output parameter (plugin -> host), contains the width of the thumbnail in number of pixels.
	FFUInt32 height;          //!< Used as output parameter (plugin -> host), contains the height of the thumbnail in number of pixels.

	void* rgbaPixelBuffer;    //!< Host provided location of where the thumbnails rgba pixels should be written. May be nullptr if the host is just querying the thumbnail size, which it needs to calculate minimum buffer size.
} GetThumbnailStruct;

//FFGLViewportStruct (for InstantiateGL)
typedef struct FFGLViewportStructTag
{
	GLuint x, y, width, height;
} FFGLViewportStruct;

//FFGLTextureStruct (for ProcessOpenGLStruct)
typedef struct FFGLTextureStructTag
{
	FFUInt32 Width, Height;
	FFUInt32 HardwareWidth, HardwareHeight;
	GLuint Handle;//the actual texture handle, from glGenTextures()
} FFGLTextureStruct;

// ProcessOpenGLStruct
typedef struct ProcessOpenGLStructTag
{
	FFUInt32 numInputTextures;
	FFGLTextureStruct** inputTextures;

	//if the host calls ProcessOpenGL with a framebuffer object actively bound
	//(as is the case when the host is capturing the plugins output to an offscreen texture)
	//the host must provide the GL handle to its EXT_framebuffer_object
	//so that the plugin can restore that binding if the plugin
	//makes use of its own FBO's for intermediate rendering
	GLuint HostFBO;
} ProcessOpenGLStruct;

// GetParameterElementNameStruct
typedef struct GetParameterElementNameStructTag
{
	FFUInt32 ParameterNumber;
	FFUInt32 ElementNumber;
} GetParameterElementNameStruct;

// GetParameterElementValueStruct
typedef struct GetParameterElementValueStructTag
{
	FFUInt32 ParameterNumber;
	FFUInt32 ElementNumber;
} GetParameterElementValueStruct;

// SetParameterElementValueStruct
typedef struct SetParameterElementValueStructTag
{
	FFUInt32 ParameterNumber;
	FFUInt32 ElementNumber;
	FFMixed NewParameterValue;
} SetParameterElementValueStruct;

// GetSeparatorElementIndexStruct
typedef struct GetSeparatorElementIndexStructTag
{
	FFUInt32 ParameterNumber;
	FFUInt32 SeparatorIndex;
} GetSeparatorElementIndexStruct;

// GetFileParameterExtensionStruct
typedef struct GetFileParameterExtensionStructTag
{
	FFUInt32 ParameterNumber;
	FFUInt32 ExtensionNumber;
} GetFileParameterExtensionStruct;

typedef struct ParamEventStructTag
{
	FFUInt32 ParameterNumber; //!< The ID of the parameter that fired the event.
	FFUInt64 eventFlags;      //!< Flags containing all events that the parameter fired since last event consume. A combination of FF_EVENT_FLAG_ flags.
} ParamEventStruct;
typedef struct GetParamEventsStructTag
{
	FFUInt32 numEvents;       //!< The number of events in the events buffer.
	ParamEventStruct* events; //!< Buffer into which the plugin will write it's pending events.
} GetParamEventsStruct;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// plugMain - The one and only exposed function
// parameters:
//  functionCode - tells the plugin which function is being called
//  inputValue - 32-bit parameter or architecture specific pointer to parameter structure (eg 64 bits on x64)
//  instanceID - the id of the instance on which you want to invoke the functionCode
//
// PLUGIN DEVELOPERS:  you shouldn't need to change this function
//
// All parameters are cast as untyped pointers and cast to appropriate
// types here
//
// All return values are cast to untyped pointers here before return to
// the host
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32

BOOL APIENTRY DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved );

extern "C" __declspec( dllexport ) FFMixed __stdcall plugMain( FFUInt32 functionCode, FFMixed inputValue, FFInstanceID instanceID );
typedef __declspec( dllimport ) FFMixed( __stdcall* FF_Main_FuncPtr )( FFUInt32, FFMixed, FFInstanceID );

#else

//linux and Mac OSX share these
FFMixed plugMain( FFUInt32 functionCode, FFMixed inputValue, FFInstanceID instanceID );
typedef FFMixed ( *FF_Main_FuncPtr )( FFUInt32 funcCode, FFMixed inputVal, FFInstanceID instanceID );

#endif

#ifndef _WIN32
}//extern "C"
#endif

#endif
