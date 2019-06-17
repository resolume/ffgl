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

CFFGLPluginManager::CFFGLPluginManager()
{
	m_iMinInputs = 0;
	m_iMaxInputs = 0;
	m_timeSupported = true;

	m_NParams = 0;
	m_pFirst = NULL;
	m_pLast = NULL;
}

CFFGLPluginManager::~CFFGLPluginManager()
{
	if( m_pFirst != NULL )
	{
		ParamInfo* pCurr = m_pFirst;
		ParamInfo* pNext = m_pFirst;

		while( pCurr != NULL )
		{
			pNext = pCurr->pNext;

			if( ( pCurr->dwType == FF_TYPE_TEXT ) &&
				( pCurr->StrDefaultValue != NULL ) )
			{
				free( pCurr->StrDefaultValue );
			}

			delete pCurr;
			pCurr = pNext;
		}
	}

	m_pFirst = NULL;
	m_pLast = NULL;
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

unsigned int CFFGLPluginManager::GetNumParams() const
{
	return m_NParams;
}

void CFFGLPluginManager::SetMinInputs( unsigned int iMinInputs )
{
	m_iMinInputs = iMinInputs;
}

void CFFGLPluginManager::SetMaxInputs( unsigned int iMaxInputs )
{
	m_iMaxInputs = iMaxInputs;
}

void CFFGLPluginManager::SetParamInfo( unsigned int pIndex, const char* pchName, unsigned int pType, float fDefaultValue )
{
	ParamInfo* pInfo = new ParamInfo;
	pInfo->ID = pIndex;
	
	pInfo->elements.resize( 1 );
	pInfo->usage = 0;

	std::string stringValue = pchName;
	memset(pInfo->Name, 0, sizeof(pInfo->Name));
	memcpy(pInfo->Name, stringValue.c_str(), std::min(sizeof(pInfo->Name), stringValue.length()));

	pInfo->dwType = pType;
	if (pType == FF_TYPE_STANDARD)
	{
		if( fDefaultValue > 1.0 )
			fDefaultValue = 1.0;
		if( fDefaultValue < 0.0 )
			fDefaultValue = 0.0;
	}

	pInfo->DefaultValue = fDefaultValue;
	pInfo->StrDefaultValue = NULL;
	pInfo->pNext = NULL;
	if( m_pFirst == NULL )
		m_pFirst = pInfo;
	if( m_pLast != NULL )
		m_pLast->pNext = pInfo;
	m_pLast = pInfo;
	m_NParams++;
}

void CFFGLPluginManager::SetBufferParamInfo( unsigned int pIndex, const char* pchName, unsigned int numElements, unsigned int usage )
{
	ParamInfo* pInfo = new ParamInfo;
	pInfo->ID = pIndex;

	pInfo->elements.resize( numElements );
	pInfo->usage = usage;

	std::string stringValue = pchName;
	memset(pInfo->Name, 0, sizeof(pInfo->Name));
	memcpy(pInfo->Name, stringValue.c_str(), std::min(sizeof(pInfo->Name), stringValue.length()));

	pInfo->dwType = FF_TYPE_BUFFER;

	pInfo->DefaultValue = 0.0f;
	pInfo->StrDefaultValue = NULL;
	pInfo->pNext = NULL;
	if( m_pFirst == NULL )
		m_pFirst = pInfo;
	if( m_pLast != NULL )
		m_pLast->pNext = pInfo;
	m_pLast = pInfo;
	m_NParams++;
}

void CFFGLPluginManager::SetOptionParamInfo( unsigned int pIndex, const char* pchName, unsigned int numElements, float defaultValue )
{
	ParamInfo* pInfo = new ParamInfo;
	pInfo->ID = pIndex;

	pInfo->elements.resize( numElements );
	pInfo->usage = FF_USAGE_STANDARD;

	std::string stringValue = pchName;
	memset(pInfo->Name, 0, sizeof(pInfo->Name));
	memcpy(pInfo->Name, stringValue.c_str(), std::min(sizeof(pInfo->Name), stringValue.length()));

	pInfo->dwType = FF_TYPE_OPTION;

	pInfo->DefaultValue = defaultValue;
	pInfo->StrDefaultValue = NULL;
	pInfo->pNext = NULL;
	if( m_pFirst == NULL )
		m_pFirst = pInfo;
	if( m_pLast != NULL )
		m_pLast->pNext = pInfo;
	m_pLast = pInfo;
	m_NParams++;
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

void CFFGLPluginManager::SetParamInfo( unsigned int pIndex, const char* pchName, unsigned int pType, bool bDefaultValue )
{
	ParamInfo* pInfo = new ParamInfo;
	pInfo->ID = pIndex;

	std::string stringValue = pchName;
	memset(pInfo->Name, 0, sizeof(pInfo->Name));
	memcpy(pInfo->Name, stringValue.c_str(), std::min(sizeof(pInfo->Name), stringValue.length()));

	pInfo->dwType = pType;
	pInfo->DefaultValue = bDefaultValue ? 1.0f : 0.0f;
	pInfo->StrDefaultValue = NULL;
	pInfo->pNext = NULL;
	if( m_pFirst == NULL )
		m_pFirst = pInfo;
	if( m_pLast != NULL )
		m_pLast->pNext = pInfo;
	m_pLast = pInfo;
	m_NParams++;
}

void CFFGLPluginManager::SetParamInfo( unsigned int dwIndex, const char* pchName, unsigned int dwType, const char* pchDefaultValue )
{
	ParamInfo* pInfo = new ParamInfo;
	pInfo->ID = dwIndex;

	pInfo->elements.resize( 1 );
	pInfo->usage = 0;

	std::string stringValue = pchName;
	memset(pInfo->Name, 0, sizeof(pInfo->Name));
	memcpy(pInfo->Name, stringValue.c_str(), std::min(sizeof(pInfo->Name), stringValue.length()));

	pInfo->dwType = dwType;
	pInfo->DefaultValue = 0;
	pInfo->StrDefaultValue = strdup( pchDefaultValue );
	pInfo->pNext = NULL;
	if( m_pFirst == NULL )
		m_pFirst = pInfo;
	if( m_pLast != NULL )
		m_pLast->pNext = pInfo;
	m_pLast = pInfo;
	m_NParams++;
}

void CFFGLPluginManager::SetTimeSupported( bool supported )
{
	m_timeSupported = supported;
}

void CFFGLPluginManager::SetParamRange(unsigned int index, float min, float max)
{
	ParamInfo* paramInfo = FindParamInfo(index);
	if (paramInfo)
		paramInfo->range = { min, max };
}

RangeStruct CFFGLPluginManager::GetParamRange(unsigned int index)
{
	RangeStruct result = { 0, 1 };
	ParamInfo* paramInfo = FindParamInfo(index);
	if (paramInfo)
		result = paramInfo->range;
	return result;
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

	if( GetParamType( dwIndex ) == FF_TYPE_TEXT )
		result.PointerValue = (void*)paramInfo->StrDefaultValue;
	else
		result.UIntValue = *(FFUInt32*)&paramInfo->DefaultValue;
	
	return result;
}

bool CFFGLPluginManager::GetTimeSupported() const
{
	return m_timeSupported;
}

CFFGLPluginManager::ParamInfo* CFFGLPluginManager::FindParamInfo( unsigned int ID )
{
	for( ParamInfo* pCurr = m_pFirst; pCurr != nullptr; pCurr = pCurr->pNext )
	{
		if( pCurr->ID == ID )
			return pCurr;
	}
	return nullptr;
}
const CFFGLPluginManager::ParamInfo* CFFGLPluginManager::FindParamInfo( unsigned int ID ) const
{
	for( const ParamInfo* pCurr = m_pFirst; pCurr != nullptr; pCurr = pCurr->pNext )
	{
		if( pCurr->ID == ID )
			return pCurr;
	}
	return nullptr;
}