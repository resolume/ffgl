////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FFGL.cpp
//
// FreeFrame is an open-source cross-platform real-time video effects plugin system.
// It provides a framework for developing video effects plugins and hosts on Windows,
// Linux and Mac OSX.
//
// FreeFrameGL (FFGL) is an extension to the FreeFrame spec to support video processing
// with OpenGL on Windows, Linux, and Mac OSX.
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
// FreeFrame 1.0 upgrade by Russell Blakeborough
// email: boblists@brightonart.org
//
// FreeFrame 1.0 - 03 upgrade
// and implementation of FreeFrame SDK methods by Gualtiero Volpe
// email: Gualtiero.Volpe@poste.it
//
// FFGL upgrade by Trey Harrison
// email: trey@harrisondigitalmedia.com
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <memory.h>
#include <assert.h>
#include <array>
#include "FFGLPluginSDK.h"
#include "FFGLThumbnailInfo.h"
#include "../glsdk_0_5_2/glload/include/gl_load.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Static and extern variables used in the FreeFrame SDK
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern CFFGLPluginInfo* g_CurrPluginInfo;

static CFFGLPlugin* s_pPrototype = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FreeFrame SDK default implementation of the FreeFrame global functions.
// Such function are called by the plugMain function, the only function a plugin exposes.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ValidateContextState();

bool InitGLExts()
{
#if defined( WIN32 )
	static bool triedInit  = false;
	static bool initResult = false;
	if( triedInit )
		return initResult;
	triedInit = true;

	if( ogl_LoadFunctions() == ogl_LOAD_FAILED )
		return false;
	initResult = true;
	return initResult;
#else
	return true;
#endif
}

void* getInfo()
{
	return (void*)( g_CurrPluginInfo->GetPluginInfo() );
}
FFResult initialise()
{
	if( g_CurrPluginInfo == NULL )
		return FF_FAIL;

	if( s_pPrototype == NULL )
	{
		//get the instantiate function pointer
		FPCREATEINSTANCEGL* pInstantiate = g_CurrPluginInfo->GetFactoryMethod();

		//call the instantiate function
		FFResult ret = pInstantiate( &s_pPrototype );

		//make sure the instantiate call worked
		if( ( ret == FF_FAIL ) || ( s_pPrototype == NULL ) )
			return FF_FAIL;

		return FF_SUCCESS;
	}

	return FF_SUCCESS;
}
FFResult deInitialise()
{
	if( s_pPrototype != NULL )
	{
		delete s_pPrototype;
		s_pPrototype = NULL;
	}
	return FF_SUCCESS;
}
unsigned int getNumParameters()
{
	if( s_pPrototype == NULL )
	{
		FFResult dwRet = initialise();
		if( dwRet == FF_FAIL )
			return FF_FAIL;
	}

	return s_pPrototype->GetNumParams();
}
char* getParameterName( unsigned int index )
{
	if( s_pPrototype == NULL )
	{
		FFResult dwRet = initialise();
		if( dwRet == FF_FAIL )
			return NULL;
	}

	return s_pPrototype->GetParamName( index );
}
FFMixed getParameterDefault( unsigned int index )
{
	FFMixed ret;
	ret.UIntValue = FF_FAIL;
	if( s_pPrototype == NULL )
	{
		FFResult dwRet = initialise();
		if( dwRet == FF_FAIL )
			return ret;
	}
	return s_pPrototype->GetParamDefault( index );
}
FFResult getPluginCaps( unsigned int index )
{
	int MinInputs = -1;
	int MaxInputs = -1;

	if( s_pPrototype == NULL )
	{
		FFResult dwRet = initialise();
		if( dwRet == FF_FAIL )
			return FF_FAIL;
	}

	switch( index )
	{
	case FF_CAP_SET_TIME:
		if( s_pPrototype->IsTimeSupported() )
			return FF_TRUE;
		else
			return FF_FALSE;
	case FF_CAP_MINIMUM_INPUT_FRAMES:
		MinInputs = s_pPrototype->GetMinInputs();
		if( MinInputs < 0 )
			return FF_FALSE;
		return MinInputs;
	case FF_CAP_MAXIMUM_INPUT_FRAMES:
		MaxInputs = s_pPrototype->GetMaxInputs();
		if( MaxInputs < 0 )
			return FF_FALSE;
		return MaxInputs;
	case FF_CAP_TOP_LEFT_TEXTURE_ORIENTATION:
		if( s_pPrototype->IsTopLeftTextureOrientationSupported() )
			return FF_TRUE;
		else
			return FF_FALSE;

	default:
		return FF_FALSE;
	}

	return FF_FAIL;
}
void* getExtendedInfo()
{
	return (void*)( g_CurrPluginInfo->GetPluginExtendedInfo() );
}
unsigned int getParameterType( unsigned int index )
{
	if( s_pPrototype == NULL )
	{
		FFResult dwRet = initialise();
		if( dwRet == FF_FAIL )
			return FF_FAIL;
	}

	return s_pPrototype->GetParamType( index );
}
void* instantiateGL( const FFGLViewportStruct* pGLViewport )
{
	if( g_CurrPluginInfo == NULL || pGLViewport == NULL )
		return (void*)FF_FAIL;

	// If the plugin is not initialized, initialize it
	if( s_pPrototype == NULL )
	{
		FFResult dwRet = initialise();

		if( ( dwRet == FF_FAIL ) || ( s_pPrototype == NULL ) )
			return (void*)FF_FAIL;
	}

	//get the instantiate function pointer
	FPCREATEINSTANCEGL* pInstantiate = g_CurrPluginInfo->GetFactoryMethod();

	CFFGLPlugin* pInstance = NULL;

	//call the instantiate function
	FFResult dwRet = pInstantiate( &pInstance );

	//make sure the instantiate call worked
	if( ( dwRet == FF_FAIL ) || ( pInstance == NULL ) )
		return (void*)FF_FAIL;

	pInstance->m_pPlugin = pInstance;

	// Initializing instance with default values
	for( unsigned int i = 0; i < s_pPrototype->GetNumParams(); ++i )
	{
		unsigned int pType = s_pPrototype->GetParamType( i );
		FFMixed pDefault   = s_pPrototype->GetParamDefault( i );
		if( pType == FF_TYPE_TEXT || pType == FF_TYPE_FILE )
			dwRet = pInstance->SetTextParameter( i, (const char*)pDefault.PointerValue );
		else
			dwRet = pInstance->SetFloatParameter( i, *(float*)&pDefault.UIntValue );

		//#ifdef FFGLTEXTFIX

		//    int type = s_pPrototype->GetParamType(i);
		//
		//    switch( type )
		//    {
		//      case FF_TYPE_TEXT:
		//		SetParameterStruct ParamStruct;
		//		ParamStruct.NewParameterValue = pDefault;
		//        dwRet = pInstance->SetParameter(&ParamStruct);
		//        break;
		//      case FF_TYPE_BUFFER:
		//        {
		//          /*int n = s_pPrototype->GetNumParamElements(DWORD(i));
		//          float * buf = new float[n];
		//          for( int i = 0; i < n; i++ )
		//            buf[i] = 0.0f; // TODO: use parameter default?
		//          ParamStruct.NewParameterValue = (DWORD)buf;
		//          dwRet = pInstance->SetParameter(&ParamStruct);
		//          delete [] buf;*/
		//        }
		//        break;
		//		default:
		//         memcpy(&ParamStruct.NewParameterValue, pValue, 4);
		//         dwRet = pInstance->SetParameter(&ParamStruct);
		//         break;
		//
		//    }

		if( dwRet == FF_FAIL )
		{
			//SetParameter failed, delete the instance
			delete pInstance;
			return (void*)FF_FAIL;
		}
	}

	if( !InitGLExts() )
		return (void*)FF_FAIL;

	//The host should pass us a context in it's default state.
	ValidateContextState();
	//call the InitGL method
	if( pInstance->InitGL( pGLViewport ) != FF_SUCCESS )
	{
		//InitGL failed, delete the instance
		pInstance->DeInitGL();
		//The plugin should return the context to it's default state.
		ValidateContextState();
		delete pInstance;

		return (void*)FF_FAIL;
	}
	else
	{
		//The plugin should return the context to it's default state.
		ValidateContextState();
		return pInstance;
	}

}
FFResult processGL( CFFGLPlugin* pPlugObj, ProcessOpenGLStruct* pogls )
{
	if( pPlugObj != NULL )
	{
		if( pogls != NULL )
		{
			// make sure Connect has been called
			if( !pPlugObj->m_isConnected )
			{
				pPlugObj->Connect();
				pPlugObj->m_isConnected = true;
			}

			//The host should pass us a context in it's default state.
			ValidateContextState();
			FFResult result = pPlugObj->ProcessOpenGL( pogls );
			//The plugin should return the context to it's default state.
			ValidateContextState();
			return result;
		}
		else
		{
			return FF_FAIL;
		}
	}
	else
	{
		return FF_FAIL;
	}
}
FFResult deInstantiateGL( void* instanceID )
{
	CFFGLPlugin* p = (CFFGLPlugin*)instanceID;

	if( p != NULL )
	{
		// Disconnect if necessary
		if( p->m_isConnected )
		{
			p->Disconnect();
			p->m_isConnected = false;
		}

		//The host should pass us a context in it's default state.
		ValidateContextState();
		p->DeInitGL();
		//The plugin should return the context to it's default state.
		ValidateContextState();
		delete p;

		return FF_SUCCESS;
	}

	return FF_FAIL;
}
FFUInt32 getNumParameterElements( unsigned int index )
{
	if( s_pPrototype == NULL )
	{
		FFResult dwRet = initialise();
		if( dwRet == FF_FAIL )
			return FF_FAIL;
	}

	return s_pPrototype->GetNumParamElements( index );
}
char* getParameterElementName( unsigned int paramIndex, unsigned int elementIndex )
{
	if( s_pPrototype == NULL )
	{
		FFResult dwRet = initialise();
		if( dwRet == FF_FAIL )
			return NULL;
	}

	return s_pPrototype->GetParamElementName( paramIndex, elementIndex );
}
FFMixed getParameterElementDefault( unsigned int paramIndex, unsigned int elementIndex )
{
	FFMixed ret;
	ret.UIntValue = FF_FAIL;
	if( s_pPrototype == NULL )
	{
		FFResult dwRet = initialise();
		if( dwRet == FF_FAIL )
			return ret;
	}
	return s_pPrototype->GetParamElementDefault( paramIndex, elementIndex );
}
FFUInt32 GetNumElementSeparators( unsigned int paramIndex )
{
	if( s_pPrototype == NULL )
	{
		FFResult dwRet = initialise();
		if( dwRet == FF_FAIL )
			return 0;
	}
	return s_pPrototype->GetNumElementSeparators( paramIndex );
}
FFUInt32 GetElementSeparatorElementIndex( unsigned int paramIndex, unsigned int separatorIndex )
{
	if( s_pPrototype == NULL )
	{
		FFResult dwRet = initialise();
		if( dwRet == FF_FAIL )
			return -1;
	}
	return s_pPrototype->GetElementSeparatorElementIndex( paramIndex, separatorIndex );
}
FFUInt32 getParameterUsage( unsigned int index )
{
	if( s_pPrototype == NULL )
	{
		FFResult dwRet = initialise();
		if( dwRet == FF_FAIL )
			return FF_FAIL;
	}

	return s_pPrototype->GetParamUsage( index );
}
const char* getPluginShortName()
{
	if( s_pPrototype == NULL )
	{
		FFResult dwRet = initialise();
		if( dwRet == FF_FAIL )
			return NULL;
	}

	const char* shortName = s_pPrototype->GetShortName();
	if( shortName == NULL )
		return NULL;

	return shortName;
}
FFMixed getParamRange( FFMixed input )
{
	FFMixed ret;
	ret.UIntValue = FF_FAIL;
	if( s_pPrototype == NULL )
	{
		FFResult dwRet = initialise();
		if( dwRet == FF_FAIL )
			return ret;
	}
	ret.UIntValue = FF_SUCCESS;
	
	GetRangeStruct* getRange = (GetRangeStruct*)input.PointerValue;

	RangeStruct range = s_pPrototype->GetParamRange( getRange->parameterNumber );
	getRange->range   = range;
	return ret;
}
FFUInt32 getThumbnail( GetThumbnailStruct& getStruct )
{
	CFFGLThumbnailInfo* thumbnailInfo = CFFGLThumbnailInfo::GetInstance();
	//It's possible that this plugin doesn't have an embedded thumbnail.
	if( thumbnailInfo == nullptr )
	{
		getStruct.width = 0;
		getStruct.height = 0;
		//There's no thumbnail available. Use same error code as old plugins that didn't support this feature
		//to make implementation on the host easier (fail = no thumbnail, success = thumbnail is available)
		return FF_FAIL;
	}

	getStruct.width = thumbnailInfo->GetWidth();
	getStruct.height = thumbnailInfo->GetHeight();
	if( getStruct.rgbaPixelBuffer != nullptr )
		memcpy( getStruct.rgbaPixelBuffer, thumbnailInfo->GetPixels(), getStruct.width * getStruct.height * 4 );

	return FF_SUCCESS;
}
FFUInt32 getNumFileParameterExtensions( unsigned int index )
{
	if( s_pPrototype == nullptr )
	{
		FFResult dwRet = initialise();
		if( dwRet == FF_FAIL )
			return FF_FAIL;
	}

	return s_pPrototype->GetNumFileParamExtensions( index );
}
char* getFileParameterExtension( unsigned int paramIndex, unsigned int extensionIndex )
{
	if( s_pPrototype == NULL )
	{
		FFResult dwRet = initialise();
		if( dwRet == FF_FAIL )
			return NULL;
	}

	return s_pPrototype->GetFileParamExtension( paramIndex, extensionIndex );
}
FFUInt32 getDefaultParameterVisibility( unsigned int index )
{
	if( s_pPrototype == NULL )
	{
		FFResult dwRet = initialise();
		if( dwRet == FF_FAIL )
			return FF_FAIL;
	}

	return s_pPrototype->GetParamVisibility( index );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implementation of plugMain, the one and only exposed function
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32

extern "C" __declspec( dllexport ) FFMixed __stdcall plugMain( FFUInt32 functionCode, FFMixed inputValue, FFInstanceID instanceID )

#elif TARGET_OS_MAC

FFMixed plugMain( FFUInt32 functionCode, FFMixed inputValue, FFInstanceID instanceID )

#elif __linux__

FFMixed plugMain( FFUInt32 functionCode, FFMixed inputValue, FFInstanceID instanceID )

#endif

{
	FFMixed retval;

	// declare pPlugObj - pointer to this instance
	CFFGLPlugin* pPlugObj;

	// typecast DWORD into pointer to a CFFGLPlugin
	pPlugObj = (CFFGLPlugin*)instanceID;

	switch( functionCode )
	{
	case FF_GET_INFO:
		retval.PointerValue = (PluginInfoStruct*)getInfo();
		break;
	case FF_INITIALISE_V2:
		retval.UIntValue = initialise();
		break;
	case FF_DEINITIALISE:
		retval.UIntValue = deInitialise();
		break;
	case FF_GET_NUM_PARAMETERS:
		retval.UIntValue = getNumParameters();
		break;
	case FF_GET_PARAMETER_NAME:
		retval.PointerValue = getParameterName( inputValue.UIntValue );
		break;
	case FF_GET_PARAMETER_DEFAULT:
		retval = getParameterDefault( inputValue.UIntValue );
		break;
	case FF_GET_PARAMETER_DISPLAY:
		if( pPlugObj != NULL )
			retval.PointerValue = pPlugObj->GetParameterDisplay( inputValue.UIntValue );
		else
			retval.PointerValue = (char*)FF_FAIL;
		break;
	case FF_SET_PARAMETER:
		if( pPlugObj != NULL )
		{
			const SetParameterStruct& setParameterStruct = *reinterpret_cast< const SetParameterStruct* >( inputValue.PointerValue );
			unsigned int paramType = getParameterType( setParameterStruct.ParameterNumber );
			if( paramType == FF_TYPE_TEXT || paramType == FF_TYPE_FILE )
			{
				retval.UIntValue = pPlugObj->SetTextParameter( setParameterStruct.ParameterNumber,
				                                               (const char*)setParameterStruct.NewParameterValue.PointerValue );
			}
			else
			{
				retval.UIntValue = pPlugObj->SetFloatParameter( setParameterStruct.ParameterNumber,
				                                                *(float*)&setParameterStruct.NewParameterValue.UIntValue );
			}
		}
		else
		{
			retval.UIntValue = FF_FAIL;
		}
		break;
	case FF_GET_PARAMETER:
		if( pPlugObj != NULL )
		{
			unsigned int paramType = getParameterType( inputValue.UIntValue );
			if( paramType == FF_TYPE_TEXT || paramType == FF_TYPE_FILE )
			{
				retval.PointerValue = pPlugObj->GetTextParameter( inputValue.UIntValue );
			}
			else
			{
				float fValue     = pPlugObj->GetFloatParameter( inputValue.UIntValue );
				retval.UIntValue = *(FFUInt32*)&fValue;
			}
		}
		else
		{
			retval.UIntValue = FF_FAIL;
		}
		break;
	case FF_GET_PLUGIN_CAPS:
		retval.UIntValue = getPluginCaps( inputValue.UIntValue );
		break;
	case FF_ENABLE_PLUGIN_CAP:
		if( pPlugObj != NULL )
		{
			switch( inputValue.UIntValue )
			{
			case FF_CAP_TOP_LEFT_TEXTURE_ORIENTATION:
				if( getPluginCaps( FF_CAP_TOP_LEFT_TEXTURE_ORIENTATION ) == FF_TRUE )
				{
					pPlugObj->HostEnabledTopLeftTextures();
				}
				else
				{
					//The host is trying to enable a capability that the plugin doesn't support, so return failure.
					retval.UIntValue = FF_FAIL;
				}
				break;
			default:
				//This is not a changable capability, so return failure.
				retval.UIntValue = FF_FAIL;
				break;
			}
		}
		else
		{
			//Capabilities need to be enabled on specific plugin instances, there's no instance so we return failure.
			retval.UIntValue = FF_FAIL;
		}
		break;
	case FF_GET_EXTENDED_INFO:
		retval.PointerValue = getExtendedInfo();
		break;
	case FF_GET_PARAMETER_TYPE:
		retval.UIntValue = getParameterType( inputValue.UIntValue );
		break;
	case FF_GET_INPUT_STATUS:
		if( pPlugObj != NULL )
			retval.UIntValue = pPlugObj->GetInputStatus( inputValue.UIntValue );
		else
			retval.UIntValue = FF_FAIL;
		break;
	case FF_PROCESS_OPENGL:
		retval.UIntValue = processGL( pPlugObj, (ProcessOpenGLStruct*)inputValue.PointerValue );
		break;
	case FF_INSTANTIATE_GL:
		retval.PointerValue = instantiateGL( (const FFGLViewportStruct*)inputValue.PointerValue );
		break;
	case FF_DEINSTANTIATE_GL:
		if( pPlugObj != NULL )
			retval.UIntValue = deInstantiateGL( pPlugObj );
		else
			retval.UIntValue = FF_FAIL;
		break;
	case FF_SET_TIME:
		if( pPlugObj != NULL )
		{
			double* inputTime = (double*)inputValue.PointerValue;
			if( inputTime != NULL )
				retval.UIntValue = pPlugObj->SetTime( *inputTime );
			else
				retval.UIntValue = FF_FAIL;
		}
		else
		{
			retval.UIntValue = FF_FAIL;
		}
		break;
	case FF_CONNECT:
		if( pPlugObj != NULL )
		{
			retval.UIntValue = pPlugObj->Connect();
			if( retval.UIntValue == FF_SUCCESS )
				pPlugObj->m_isConnected = true;
		}
		else
		{
			retval.UIntValue = FF_FAIL;
		}
		break;
	case FF_DISCONNECT:
		if( pPlugObj != NULL )
		{
			retval.UIntValue = pPlugObj->Disconnect();
			if( retval.UIntValue == FF_SUCCESS )
				pPlugObj->m_isConnected = false;
		}
		else
		{
			retval.UIntValue = FF_FAIL;
		}
		break;
	case FF_RESIZE:
		if( pPlugObj != NULL )
		{
			retval.UIntValue = pPlugObj->Resize( (const FFGLViewportStruct*)inputValue.PointerValue );
		}
		else
		{
			retval.UIntValue = FF_FAIL;
		}
		break;
	case FF_GET_NUM_PARAMETER_ELEMENTS:
		retval.UIntValue = getNumParameterElements( inputValue.UIntValue );
		break;
	case FF_GET_PARAMETER_ELEMENT_NAME:
	{
		const GetParameterElementNameStruct* arguments = (const GetParameterElementNameStruct*)inputValue.PointerValue;
		retval.PointerValue                            = getParameterElementName( arguments->ParameterNumber, arguments->ElementNumber );
		break;
	}
	case FF_GET_PARAMETER_ELEMENT_DEFAULT:
	{
		const GetParameterElementValueStruct* arguments = (const GetParameterElementValueStruct*)inputValue.PointerValue;
		retval                                          = getParameterElementDefault( arguments->ParameterNumber, arguments->ElementNumber );
		break;
	}
	case FF_GET_NUM_ELEMENT_SEPARATORS:
	{
		retval.UIntValue = GetNumElementSeparators( inputValue.UIntValue );
		break;
	}
	case FF_GET_SEPARATOR_ELEMENT_INDEX:
	{
		const GetSeparatorElementIndexStructTag* arguments = (const GetSeparatorElementIndexStructTag*)inputValue.PointerValue;
		retval.UIntValue = GetElementSeparatorElementIndex( arguments->ParameterNumber, arguments->SeparatorIndex );
		break;
	}
	case FF_SET_PARAMETER_ELEMENT_VALUE:
		if( pPlugObj != NULL )
		{
			const SetParameterElementValueStruct* arguments = (const SetParameterElementValueStruct*)inputValue.PointerValue;
			retval.UIntValue                                = pPlugObj->SetParamElementValue( arguments->ParameterNumber, arguments->ElementNumber, *(float*)&arguments->NewParameterValue.UIntValue );
		}
		break;
	case FF_GET_PARAMETER_USAGE:
		retval.UIntValue = getParameterUsage( inputValue.UIntValue );
		break;
	case FF_GET_PLUGIN_SHORT_NAME:
		retval.PointerValue = (void*)getPluginShortName();
		break;
	case FF_SET_BEATINFO:
		if( pPlugObj != NULL )
		{
			const SetBeatinfoStruct* beatInfo = reinterpret_cast< const SetBeatinfoStruct* >( inputValue.PointerValue );
			float bpm                         = *(float*)&beatInfo->bpm;
			float barPhase                    = *(float*)&beatInfo->barPhase;
			pPlugObj->SetBeatInfo( bpm, barPhase );
			retval.UIntValue = FF_SUCCESS;
		}
		else
		{
			retval.UIntValue = FF_FAIL;
		}

		break;
	case FF_SET_HOSTINFO:
		if( pPlugObj != NULL )
		{
			const SetHostinfoStructTag* hostInfo = reinterpret_cast< const SetHostinfoStructTag* >( inputValue.PointerValue );
			pPlugObj->SetHostInfo( hostInfo->name, hostInfo->version );
			retval.UIntValue = FF_SUCCESS;
		}
		else
		{
			retval.UIntValue = FF_FAIL;
		}

		break;

	case FF_SET_SAMPLERATE:
		if( pPlugObj != NULL )
		{
			pPlugObj->SetSampleRate( inputValue.UIntValue );
			retval.UIntValue = FF_SUCCESS;
		}
		else
		{
			retval.UIntValue = FF_FAIL;
		}

		break;
	case FF_GET_RANGE:
		retval = getParamRange( inputValue );
		break;

	case FF_GET_THUMBNAIL:
		if( inputValue.PointerValue != nullptr )
			retval.UIntValue = getThumbnail( *reinterpret_cast< GetThumbnailStruct* >( inputValue.PointerValue ) );
		else
			retval.UIntValue = FF_FAIL;
		break;

	case FF_GET_NUM_FILE_PARAMETER_EXTENSIONS:
		retval.UIntValue = getNumFileParameterExtensions( inputValue.UIntValue );
		break;
	case FF_GET_FILE_PARAMETER_EXTENSION:
	{
		const GetFileParameterExtensionStruct* arguments = reinterpret_cast< const GetFileParameterExtensionStruct* >( inputValue.PointerValue );
		retval.PointerValue = getFileParameterExtension( arguments->ParameterNumber, arguments->ExtensionNumber );
		break;
	}

	case FF_GET_PRAMETER_VISIBILITY:
	{
		if( pPlugObj != nullptr )
			retval.UIntValue = pPlugObj->GetParamVisibility( inputValue.UIntValue );
		else
			retval.UIntValue = getDefaultParameterVisibility( inputValue.UIntValue );
		break;
	}

	case FF_GET_PARAMETER_EVENTS:
	{
		GetParamEventsStruct& eventsBuffer = *reinterpret_cast< GetParamEventsStruct* >( inputValue.PointerValue );
		//Events orignate from plugin instances so if no instance exists for this request we cannot fullfill it.
		if( pPlugObj != nullptr )
		{
			FFUInt32 numPendingEvents = pPlugObj->GetNumPendingParamEvents();
			//Hosts are allowed to query the number of events that are pending by passing in a nullptr for the events buffer.
			//In that case we're outputting the number of pending events in the numEvents field and dont try to write any events into the event buffer.
			if( eventsBuffer.events == nullptr )
			{
				eventsBuffer.numEvents = numPendingEvents;
				retval.UIntValue = FF_SUCCESS;
			}
			else
			{
				//The host has provided a buffer to write events in to. We'll be writing our events into the buffer and output the
				//number of events we've written in there.
				eventsBuffer.numEvents = pPlugObj->ConsumeParamEvents( eventsBuffer.events, eventsBuffer.numEvents );
				retval.UIntValue = FF_SUCCESS;
			}
		}
		else
		{
			retval.UIntValue = FF_FAIL;
		}
		break;
	}

	//Previously used function codes that are no longer supported:
	//case FF_INITIALISE:
	/**
	 * We're dropping the old FFGL 1.6 and lower initialise here. FFGL 2.0 removed old stuff and made support for newer stuff mandatory
	 * so hosts need a way to know they cannot use this plugin if they're dependant on the old behaviour. If the host isn't dependant on the old
	 * behaviour it will have to update to build using the FFGL 2.0 sdk and instead invoke the initialise_v2 opcode. This way
	 * the plugin and host both agree that it's okay not to support the old behaviour.
	 */
	//case FF_INSTANTIATE:
	//case FF_DEINSTANTIATE:
	//case FF_PROCESSFRAME:
	//case FF_PROCESSFRAMECOPY:
	default:
		retval.UIntValue = FF_FAIL;
		break;
	}

	return retval;
}

/**
 * The FFGL host provides us with a context in the default state. We have to return a context in the default
 * state back to the host. In previous FFGL versions this was also the convention, but it was never actually checked.
 * Since previous FFGL versions were based on OpenGL 1.0 the host had the ability to isolate these state changes
 * so that it'll stay unaffected by what a plugin does. Due to the move to OpenGL 4.1 the host no longer has this ability
 * so it becomes extra important to enforce this convention. The problem when plugins dont restore the state is that
 * the host's rendering may be affected. For example if the plugin keeps a VBO bound but the hosts renders something assuming
 * no vbo is bound, it'll start fetching from the plugin's vbo.
 */
void ValidateContextState()
{
#if defined( _DEBUG )
	GLint glInt[ 4 ];
	GLboolean glBool[ 4 ];

	//Please use the ScopedShaderBinding to automatically unbind your shaders.
	glGetIntegerv( GL_CURRENT_PROGRAM, glInt );
	assert( glInt[ 0 ] == 0 );

	//Please use the ScopedSamplerActivation to automatically return the active sampler to the default state.
	glGetIntegerv( GL_ACTIVE_TEXTURE, glInt );
	assert( glInt[ 0 ] == GL_TEXTURE0 );

	struct TextureType
	{
		GLenum target;
		GLenum binding;
	};
	const std::array< TextureType, 11 > TEXTURE_TYPES =
	{
		TextureType{ GL_TEXTURE_1D, GL_TEXTURE_BINDING_1D },
		TextureType{ GL_TEXTURE_2D, GL_TEXTURE_BINDING_2D },
		TextureType{ GL_TEXTURE_3D, GL_TEXTURE_BINDING_3D },
		TextureType{ GL_TEXTURE_1D_ARRAY, GL_TEXTURE_BINDING_1D_ARRAY },
		TextureType{ GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BINDING_2D_ARRAY },
		TextureType{ GL_TEXTURE_RECTANGLE, GL_TEXTURE_BINDING_RECTANGLE },
		TextureType{ GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BINDING_CUBE_MAP },
		TextureType{ GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_BINDING_CUBE_MAP_ARRAY },
		TextureType{ GL_TEXTURE_BUFFER, GL_TEXTURE_BINDING_BUFFER },
		TextureType{ GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_BINDING_2D_MULTISAMPLE },
		TextureType{ GL_TEXTURE_2D_MULTISAMPLE_ARRAY, GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY }
	};
	GLint numSamplers;
	glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &numSamplers );
	for( const auto& pair : TEXTURE_TYPES )
	{
		for( GLint sampler = 0; sampler < numSamplers; ++sampler )
		{
			//Please use the ScopedTextureBinding to automatically unbind textures after you're done with them.
			glGetIntegerv( pair.binding, glInt );
			assert( glInt[ 0 ] == 0 );
		}
	}

	//Please use the ScopedVBOBinding to automatically unbind your vertex buffers.
	glGetIntegerv( GL_ARRAY_BUFFER_BINDING, glInt );
	assert( glInt[ 0 ] == 0 );

	//Please use the ScopedIBOBinding to automatically unbind your index buffers.
	glGetIntegerv( GL_ELEMENT_ARRAY_BUFFER_BINDING, glInt );
	assert( glInt[ 0 ] == 0 );

	//Please use the ScopedUBOBinding to automatically unbind your uniform buffers.
	glGetIntegerv( GL_UNIFORM_BUFFER_BINDING, glInt );
	assert( glInt[ 0 ] == 0 );





	//We have no scoped bindings for the render state. You need to manually return these to the context default state.
	glGetIntegerv( GL_POLYGON_MODE, glInt );
	assert( glInt[ 0 ] == GL_FILL );

	assert( glIsEnabled( GL_CULL_FACE ) == GL_FALSE );
	glGetIntegerv( GL_FRONT_FACE, glInt );
	assert( glInt[ 0 ] == GL_CCW );

	assert( glIsEnabled( GL_BLEND ) == GL_FALSE );

	glGetIntegerv( GL_BLEND_EQUATION_RGB, glInt );
	assert( glInt[ 0 ] == GL_FUNC_ADD );
	glGetIntegerv( GL_BLEND_EQUATION_ALPHA, glInt );
	assert( glInt[ 0 ] == GL_FUNC_ADD );

	glGetIntegerv( GL_BLEND_SRC_RGB, glInt );
	assert( glInt[ 0 ] == GL_ONE );
	glGetIntegerv( GL_BLEND_SRC_ALPHA, glInt );
	assert( glInt[ 0 ] == GL_ONE );
	glGetIntegerv( GL_BLEND_DST_RGB, glInt );
	assert( glInt[ 0 ] == GL_ZERO );
	glGetIntegerv( GL_BLEND_DST_ALPHA, glInt );
	assert( glInt[ 0 ] == GL_ZERO );

	glGetBooleanv( GL_DEPTH_WRITEMASK, glBool );
	assert( glBool[ 0 ] == GL_TRUE );

	assert( glIsEnabled( GL_DEPTH_TEST ) == GL_FALSE );
	glGetIntegerv( GL_DEPTH_FUNC, glInt );
	assert( glInt[ 0 ] == GL_LESS );

	glGetBooleanv( GL_COLOR_WRITEMASK, glBool );
	assert( glBool[ 0 ] == GL_TRUE );
	assert( glBool[ 1 ] == GL_TRUE );
	assert( glBool[ 2 ] == GL_TRUE );
	assert( glBool[ 3 ] == GL_TRUE );
#endif
}