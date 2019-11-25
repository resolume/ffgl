//
// Copyright (c) 2004 - InfoMus Lab - DIST - University of Genova
//
// InfoMus Lab (Laboratorio di Informatica Musicale)
// DIST - University of Genova
//
// http://www.infomus.dist.unige.it
// news://infomus.dist.unige.it
// mailto:staff@infomus.dist.unige.it
//
// Developer: Gualtiero Volpe
// mailto:volpe@infomus.dist.unige.it
//
// Modified: Oct 25 2006 by Trey Harrison
// email:trey@harrisondigitalmedia.com
//
// Last modified: Oct 01 2018 by Menno Vink
// email:menno@resolume.com

#include "FFGLPluginManager.h"
#include "FFGLPluginSDK.h"

#include <stdlib.h>
#include <memory.h>
#include <algorithm>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFFGLPluginManager constructor and destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CFFGLPluginManager::CFFGLPluginManager( bool supportTopLeftTextureOrientation ) :
	m_iMinInputs( 0 ),
	m_iMaxInputs( 0 ),
	m_timeSupported( true ),
	m_topLeftTextureOrientationSupported( supportTopLeftTextureOrientation ),
	textureOrientation( TextureOrientation::BOTTOM_LEFT )
{
}
CFFGLPluginManager::~CFFGLPluginManager()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFFGLPluginManager methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned int CFFGLPluginManager::GetMinInputs() const
{
	return m_iMinInputs;
}
unsigned int CFFGLPluginManager::GetMaxInputs() const
{
	return m_iMaxInputs;
}

bool CFFGLPluginManager::IsTimeSupported() const
{
	return m_timeSupported;
}
bool CFFGLPluginManager::IsTopLeftTextureOrientationSupported() const
{
	return m_topLeftTextureOrientationSupported;
}
void CFFGLPluginManager::HostEnabledTopLeftTextures()
{
	textureOrientation = TextureOrientation::TOP_LEFT;
}

unsigned int CFFGLPluginManager::GetNumParams() const
{
	return static_cast< unsigned int >( params.size() );
}
char* CFFGLPluginManager::GetParamName( unsigned int dwIndex )
{
	ParamInfo* paramInfo = FindParamInfo( dwIndex );
	if( paramInfo == nullptr )
		return nullptr;

	return paramInfo->Name;
}
unsigned int CFFGLPluginManager::GetParamType( unsigned int dwIndex ) const
{
	const ParamInfo* paramInfo = FindParamInfo( dwIndex );
	if( paramInfo == nullptr )
		return FF_FAIL;

	return paramInfo->dwType;
}
unsigned int CFFGLPluginManager::GetParamUsage( unsigned int dwIndex ) const
{
	const ParamInfo* paramInfo = FindParamInfo( dwIndex );
	if( paramInfo == nullptr )
		return FF_FAIL;

	return paramInfo->usage;
}
FFMixed CFFGLPluginManager::GetParamDefault( unsigned int dwIndex ) const
{
	FFMixed result;
	result.UIntValue = FF_FAIL;
	const ParamInfo* paramInfo = FindParamInfo( dwIndex );
	if( paramInfo == nullptr )
		return result;

	if( GetParamType( dwIndex ) == FF_TYPE_TEXT || GetParamType( dwIndex ) == FF_TYPE_FILE )
		result.PointerValue = (void*)paramInfo->defaultStringVal.c_str();
	else
		result.UIntValue = *(FFUInt32*)&paramInfo->defaultFloatVal;

	return result;
}
FFUInt32 CFFGLPluginManager::GetParamVisibility( unsigned int dwIndex ) const
{
	const ParamInfo* paramInfo = FindParamInfo( dwIndex );
	return paramInfo != nullptr ? paramInfo->visibleInUI : FF_FAIL;
}

unsigned int CFFGLPluginManager::GetNumParamElements( unsigned int dwIndex ) const
{
	const ParamInfo* paramInfo = FindParamInfo( dwIndex );
	if( paramInfo == nullptr )
		return FF_FAIL;

	return (unsigned int)paramInfo->elements.size();
}
char* CFFGLPluginManager::GetParamElementName( unsigned int dwIndex, unsigned int elIndex )
{
	ParamInfo* paramInfo = FindParamInfo( dwIndex );
	if( paramInfo == nullptr )
		return nullptr;

	if( elIndex >= paramInfo->elements.size() )
		return nullptr;

	/**
	 * Have to const-cast here because ffgl is implemented using a single interface function and thus we cannot differentiate
	 * between constant and non constant pointers. This is also a problem of returning a pointer to our string rather than outputting
	 * our string into the caller's buffer.
	 */
	return const_cast< char* >( paramInfo->elements[ elIndex ].name.c_str() );
}
FFMixed CFFGLPluginManager::GetParamElementDefault( unsigned int dwIndex, unsigned int elIndex ) const
{
	FFMixed result;
	result.UIntValue = FF_FAIL;
	const ParamInfo* paramInfo = FindParamInfo( dwIndex );
	if( paramInfo == nullptr )
		return result;
	if( elIndex >= paramInfo->elements.size() )
		return result;

	result.UIntValue = *(unsigned int*)&paramInfo->elements[ elIndex ].value;

	return result;
}
FFUInt32 CFFGLPluginManager::SetParamElementValue( unsigned int dwIndex, unsigned int elIndex, float newValue )
{
	ParamInfo* paramInfo = FindParamInfo( dwIndex );
	if( paramInfo == nullptr )
		return FF_FAIL;

	if( elIndex >= paramInfo->elements.size() )
		return FF_FAIL;

	paramInfo->elements[ elIndex ].value = newValue;
	return FF_SUCCESS;
}
FFUInt32 CFFGLPluginManager::GetNumElementSeparators( unsigned int dwIndex )
{
	ParamInfo* paramInfo = FindParamInfo( dwIndex );
	return paramInfo != nullptr ? static_cast< FFUInt32 >( paramInfo->elementSeparators.size() ) : 0;
}
FFUInt32 CFFGLPluginManager::GetElementSeparatorElementIndex( unsigned int dwIndex, unsigned int separatorIndex )
{
	ParamInfo* paramInfo = FindParamInfo( dwIndex );
	if( paramInfo != nullptr && separatorIndex < paramInfo->elementSeparators.size() )
		return paramInfo->elementSeparators[ separatorIndex ].beforeIndex;
	else
		return -1;
}

unsigned int CFFGLPluginManager::GetNumFileParamExtensions( unsigned int index ) const
{
	const ParamInfo* paramInfo = FindParamInfo( index );
	if( paramInfo == nullptr )
		return 0;

	return (unsigned int)paramInfo->supportedExtensions.size();
}
char* CFFGLPluginManager::GetFileParamExtension( unsigned int paramIndex, unsigned int extensionIndex ) const
{
	const ParamInfo* paramInfo = FindParamInfo( paramIndex );
	if( paramInfo == nullptr )
		return nullptr;

	if( extensionIndex >= paramInfo->supportedExtensions.size() )
		return nullptr;

	/**
	 * Have to const-cast here because ffgl is implemented using a single interface function and thus we cannot differentiate
	 * between constant and non constant pointers. This is also a problem of returning a pointer to our string rather than outputting
	 * our string into the caller's buffer.
	 */
	return const_cast< char* >( paramInfo->supportedExtensions[ extensionIndex ].c_str() );
}

RangeStruct CFFGLPluginManager::GetParamRange( unsigned int index )
{
	RangeStruct result   = { 0, 1 };
	ParamInfo* paramInfo = FindParamInfo( index );
	if( paramInfo )
		result = paramInfo->range;
	return result;
}

FFUInt32 CFFGLPluginManager::GetNumPendingParamEvents() const
{
	FFUInt32 numPendingEvents = 0;
	for( const ParamInfo& param : params )
	{
		if( param.pendingEventFlags != 0 )
			numPendingEvents++;
	}
	return numPendingEvents;
}
FFUInt32 CFFGLPluginManager::ConsumeParamEvents( ParamEventStruct* events, FFUInt32 maxNumEvents )
{
	FFUInt32 numEventsConsumed = 0;
	for( size_t index = 0; index < params.size() && numEventsConsumed < maxNumEvents; ++index )
	{
		if( params[ index ].pendingEventFlags != 0 )
		{
			events[ numEventsConsumed ].ParameterNumber = params[ index ].ID;
			events[ numEventsConsumed ].eventFlags = params[ index ].pendingEventFlags;
			params[ index ].pendingEventFlags = 0;
			numEventsConsumed++;
		}
	}
	return numEventsConsumed;
}

void CFFGLPluginManager::SetMinInputs( unsigned int iMinInputs )
{
	m_iMinInputs = iMinInputs;
}
void CFFGLPluginManager::SetMaxInputs( unsigned int iMaxInputs )
{
	m_iMaxInputs = iMaxInputs;
}

void CFFGLPluginManager::SetTimeSupported( bool supported )
{
	m_timeSupported = supported;
}

void CFFGLPluginManager::SetParamInfo( unsigned int pIndex, const char* pchName, unsigned int pType, float fDefaultValue )
{
	ParamInfo pInfo;
	pInfo.ID = pIndex;
	
	pInfo.elements.resize( 1 );
	pInfo.usage = 0;

	std::string stringValue = pchName;
	memset(pInfo.Name, 0, sizeof(pInfo.Name));
	memcpy(pInfo.Name, stringValue.c_str(), std::min(sizeof(pInfo.Name), stringValue.length()));

	pInfo.dwType = pType;
	if (pType == FF_TYPE_STANDARD)
	{
		if( fDefaultValue > 1.0 )
			fDefaultValue = 1.0;
		if( fDefaultValue < 0.0 )
			fDefaultValue = 0.0;
	}

	pInfo.defaultFloatVal = fDefaultValue;
	params.push_back( pInfo );
}
void CFFGLPluginManager::SetParamInfo( unsigned int pIndex, const char* pchName, unsigned int pType, bool bDefaultValue )
{
	ParamInfo pInfo;
	pInfo.ID = pIndex;

	std::string stringValue = pchName;
	memset(pInfo.Name, 0, sizeof(pInfo.Name));
	memcpy(pInfo.Name, stringValue.c_str(), std::min(sizeof(pInfo.Name), stringValue.length()));

	pInfo.dwType = pType;
	pInfo.defaultFloatVal = bDefaultValue ? 1.0f : 0.0f;
	params.push_back( pInfo );
}
void CFFGLPluginManager::SetParamInfo( unsigned int dwIndex, const char* pchName, unsigned int dwType, const char* pchDefaultValue )
{
	ParamInfo pInfo;
	pInfo.ID = dwIndex;

	pInfo.elements.resize( 1 );
	pInfo.usage = 0;

	std::string stringValue = pchName;
	memset(pInfo.Name, 0, sizeof(pInfo.Name));
	memcpy(pInfo.Name, stringValue.c_str(), std::min(sizeof(pInfo.Name), stringValue.length()));

	pInfo.dwType = dwType;
	pInfo.defaultStringVal = pchDefaultValue;
	params.push_back( pInfo );
}

void CFFGLPluginManager::SetBufferParamInfo( unsigned int pIndex, const char* pchName, unsigned int numElements, unsigned int usage )
{
	ParamInfo pInfo;
	pInfo.ID = pIndex;

	pInfo.elements.resize( numElements );
	pInfo.usage = usage;

	std::string stringValue = pchName;
	memset(pInfo.Name, 0, sizeof(pInfo.Name));
	memcpy(pInfo.Name, stringValue.c_str(), std::min(sizeof(pInfo.Name), stringValue.length()));

	pInfo.dwType = FF_TYPE_BUFFER;

	pInfo.defaultFloatVal = 0.0f;
	params.push_back( pInfo );
}
void CFFGLPluginManager::SetOptionParamInfo( unsigned int pIndex, const char* pchName, unsigned int numElements, float defaultValue )
{
	ParamInfo pInfo;
	pInfo.ID = pIndex;

	pInfo.elements.resize( numElements );
	pInfo.usage = FF_USAGE_STANDARD;

	std::string stringValue = pchName;
	memset(pInfo.Name, 0, sizeof(pInfo.Name));
	memcpy(pInfo.Name, stringValue.c_str(), std::min(sizeof(pInfo.Name), stringValue.length()));

	pInfo.dwType = FF_TYPE_OPTION;

	pInfo.defaultFloatVal = defaultValue;
	params.push_back( pInfo );
}
void CFFGLPluginManager::SetParamElementInfo( unsigned int paramID, unsigned int elementIndex, const char* elementName, float elementValue )
{
	ParamInfo* paramInfo = FindParamInfo( paramID );
	if( paramInfo == nullptr )
		return;

	if( elementIndex >= paramInfo->elements.size() )
		return;
	
	paramInfo->elements[ elementIndex ].name = elementName;
	paramInfo->elements[ elementIndex ].value = elementValue;
}

void CFFGLPluginManager::AddElementSeparator( unsigned int paramID, unsigned int beforeElementIndex )
{
	ParamInfo* paramInfo = FindParamInfo( paramID );
	if( paramInfo == nullptr )
		return;

	paramInfo->elementSeparators.push_back( ParamInfo::ElementSeparator{ beforeElementIndex } );
}

void CFFGLPluginManager::SetFileParamInfo( unsigned int index, const char* pchName, std::vector< std::string > supportedExtensions )
{
	ParamInfo pInfo;
	pInfo.ID = index;

	std::string stringValue = pchName;
	memset( pInfo.Name, 0, sizeof( pInfo.Name ) );
	memcpy( pInfo.Name, stringValue.c_str(), std::min( sizeof( pInfo.Name ), stringValue.length() ) );

	pInfo.dwType = FF_TYPE_FILE;

	pInfo.usage = 0;

	pInfo.supportedExtensions = std::move( supportedExtensions );
	params.push_back( pInfo );
}

void CFFGLPluginManager::SetParamVisibility( unsigned int paramID, bool shouldBeVisible )
{
	ParamInfo* paramInfo = FindParamInfo( paramID );
	if( paramInfo != nullptr )
		paramInfo->visibleInUI = shouldBeVisible;
}
void CFFGLPluginManager::SetParamRange( unsigned int index, float min, float max )
{
	ParamInfo* paramInfo = FindParamInfo( index );
	if( paramInfo != nullptr )
		paramInfo->range = { min, max };
}

void CFFGLPluginManager::RaiseParamEvent( unsigned int paramID, FFUInt64 eventToRaise )
{
	ParamInfo* paramInfo = FindParamInfo( paramID );
	if( paramInfo != nullptr )
		paramInfo->pendingEventFlags |= eventToRaise;
}

CFFGLPluginManager::ParamInfo* CFFGLPluginManager::FindParamInfo( unsigned int ID )
{
	for( ParamInfo& param : params )
	{
		if( param.ID == ID )
			return &param;
	}

	return nullptr;
}
const CFFGLPluginManager::ParamInfo* CFFGLPluginManager::FindParamInfo( unsigned int ID ) const
{
	for( const ParamInfo& param : params )
	{
		if( param.ID == ID )
			return &param;
	}

	return nullptr;
}
CFFGLPluginManager::TextureOrientation CFFGLPluginManager::GetTextureOrientation() const
{
	return textureOrientation;
}