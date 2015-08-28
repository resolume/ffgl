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
// Last modified: Oct 25 2006 by Trey Harrison
// email:trey@harrisondigitalmedia.com

#include "FFGLPluginManager.h"
#include "FFGLPluginSDK.h"

#include <stdlib.h> 
#include <memory.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFFGLPluginManager constructor and destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CFFGLPluginManager::CFFGLPluginManager()
{
	m_iMinInputs = 0;
	m_iMaxInputs = 0;
  m_timeSupported = 0;

	m_NParams = 0;
	m_pFirst = NULL;
	m_pLast = NULL;
}

CFFGLPluginManager::~CFFGLPluginManager()
{
	if (m_pFirst != NULL)
  {
		ParamInfo* pCurr = m_pFirst;
		ParamInfo* pNext = m_pFirst;

		while (pCurr != NULL)
    {
			pNext = pCurr->pNext;

			if ( (pCurr->dwType == FF_TYPE_TEXT) &&
				 (pCurr->StrDefaultValue != NULL) )
			{
				free(pCurr->StrDefaultValue);
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
void CFFGLPluginManager::SetMinInputs(unsigned int iMinInputs)
{
	m_iMinInputs = iMinInputs;
}

void CFFGLPluginManager::SetMaxInputs(unsigned int iMaxInputs)
{
	m_iMaxInputs = iMaxInputs;
}

void CFFGLPluginManager::SetParamInfo(unsigned int pIndex, const char* pchName, unsigned int pType, float fDefaultValue)
{
	ParamInfo* pInfo = new ParamInfo;
	pInfo->ID = pIndex;

#ifdef FFGL_EXT
	pInfo->numElements = 1;
    pInfo->usage = 0;
#endif
	
	bool bEndFound = false;
	for (int i = 0; i < 16; ++i) {
		if (pchName[i] == 0) bEndFound = true;
		pInfo->Name[i] = (bEndFound) ?  0 : pchName[i];
	}
	
	pInfo->dwType = pType;
	if (fDefaultValue > 1.0) fDefaultValue = 1.0;
	if (fDefaultValue < 0.0) fDefaultValue = 0.0;
	pInfo->DefaultValue = fDefaultValue;
	pInfo->StrDefaultValue = NULL;
	pInfo->pNext = NULL;
	if (m_pFirst == NULL) m_pFirst = pInfo; 
	if (m_pLast != NULL) m_pLast->pNext = pInfo;
	m_pLast = pInfo;
	m_NParams++;
}

#ifdef FFGL_EXT
void CFFGLPluginManager::SetBufferParamInfo(unsigned int pIndex, const char* pchName,  unsigned int numElements, unsigned int usage )
{
	ParamInfo* pInfo = new ParamInfo;
	pInfo->ID = pIndex;
	
	pInfo->numElements = numElements;
	pInfo->usage = usage;

	bool bEndFound = false;
	for (int i = 0; i < 16; ++i) {
		if (pchName[i] == 0) bEndFound = true;
		pInfo->Name[i] = (bEndFound) ?  0 : pchName[i];
	}
	
	pInfo->dwType = FF_TYPE_BUFFER;

	pInfo->DefaultValue = 0.0f;
	pInfo->StrDefaultValue = NULL;
	pInfo->pNext = NULL;
	if (m_pFirst == NULL) m_pFirst = pInfo; 
	if (m_pLast != NULL) m_pLast->pNext = pInfo;
	m_pLast = pInfo;
	m_NParams++;
}

void CFFGLPluginManager::SetOptionParamInfo(unsigned int pIndex, const char* pchName,  unsigned int numElements, int defaultValue)
{
	ParamInfo* pInfo = new ParamInfo;
	pInfo->ID = pIndex;
	
  pInfo->numElements = numElements;
  pInfo->usage = FF_USAGE_STANDARD;
  
	bool bEndFound = false;
	for (int i = 0; i < 16; ++i) {
		if (pchName[i] == 0) bEndFound = true;
		pInfo->Name[i] = (bEndFound) ?  0 : pchName[i];
	}
	
	pInfo->dwType = FF_TYPE_OPTION;
  
	pInfo->DefaultValue = defaultValue;
	pInfo->StrDefaultValue = NULL;
	pInfo->pNext = NULL;
	if (m_pFirst == NULL) m_pFirst = pInfo; 
	if (m_pLast != NULL) m_pLast->pNext = pInfo;
	m_pLast = pInfo;
	m_NParams++;

}
#endif


void CFFGLPluginManager::SetParamInfo(unsigned int pIndex, const char* pchName, unsigned int pType, bool bDefaultValue)
{
	ParamInfo* pInfo = new ParamInfo;
	pInfo->ID = pIndex;
	
	bool bEndFound = false;
	for (int i = 0; i < 16; ++i) {
		if (pchName[i] == 0) bEndFound = true;
		pInfo->Name[i] = (bEndFound) ?  0 : pchName[i];
	}
	
	pInfo->dwType = pType;
	pInfo->DefaultValue = bDefaultValue ? 1.0f : 0.0f;
	pInfo->StrDefaultValue = NULL;
	pInfo->pNext = NULL;
	if (m_pFirst == NULL) m_pFirst = pInfo; 
	if (m_pLast != NULL) m_pLast->pNext = pInfo;
	m_pLast = pInfo;
	m_NParams++;
}

void CFFGLPluginManager::SetParamInfo(unsigned int dwIndex, const char* pchName, unsigned int dwType, const char* pchDefaultValue)
{
	ParamInfo* pInfo = new ParamInfo;
	pInfo->ID = dwIndex;

#ifdef FFGL_EXT
	pInfo->numElements = 1;
    pInfo->usage = 0;
#endif
	
	bool bEndFound = false;
	for (int i = 0; i < 16; ++i) {
		if (pchName[i] == 0) bEndFound = true;
		pInfo->Name[i] = (bEndFound) ?  0 : pchName[i];
	}

	pInfo->dwType = dwType;
	pInfo->DefaultValue = 0;
	pInfo->StrDefaultValue = strdup(pchDefaultValue);
	pInfo->pNext = NULL;
	if (m_pFirst == NULL) m_pFirst = pInfo; 
	if (m_pLast != NULL) m_pLast->pNext = pInfo;
	m_pLast = pInfo;
	m_NParams++;
}

void CFFGLPluginManager::SetTimeSupported(bool supported)
{
  m_timeSupported = supported;
}

char* CFFGLPluginManager::GetParamName(unsigned int dwIndex) const
{
	ParamInfo* pCurr = m_pFirst;
	bool bFound = false;
	while (pCurr != NULL) {
		if (pCurr->ID == dwIndex) {
			bFound = true;
			break;
		}
		pCurr = pCurr->pNext;
	}
	if (bFound) return pCurr->Name;
	return NULL;
}
	
unsigned int CFFGLPluginManager::GetParamType(unsigned int dwIndex) const
{
	ParamInfo* pCurr = m_pFirst;
	bool bFound = false;
	while (pCurr != NULL) {
		if (pCurr->ID == dwIndex) {
			bFound = true;
			break;
		}
		pCurr = pCurr->pNext;
	}
	if (bFound) return pCurr->dwType;
	return FF_FAIL;
}

#ifdef FFGL_EXT
unsigned int CFFGLPluginManager::GetNumParamElements(unsigned int dwIndex) const
{
	ParamInfo* pCurr = m_pFirst;
	bool bFound = false;
	while (pCurr != NULL) {
		if (pCurr->ID == dwIndex) {
			bFound = true;
			break;
		}
		pCurr = pCurr->pNext;
	}
	if (bFound) return pCurr->numElements;
	return FF_FAIL;
}

unsigned int CFFGLPluginManager::GetParamUsage(unsigned int dwIndex) const
{
	ParamInfo* pCurr = m_pFirst;
	bool bFound = false;
	while (pCurr != NULL) {
		if (pCurr->ID == dwIndex) {
			bFound = true;
			break;
		}
		pCurr = pCurr->pNext;
	}
	if (bFound) return pCurr->usage;
	return FF_FAIL;
}


#endif

FFMixed CFFGLPluginManager::GetParamDefault(unsigned int dwIndex) const
{
  FFMixed result;
	ParamInfo* pCurr = m_pFirst;
	bool bFound = false;
	while (pCurr != NULL) {
		if (pCurr->ID == dwIndex) {
			bFound = true;
			break;
		}
		pCurr = pCurr->pNext;
	}
	if (bFound) {
		if (GetParamType(dwIndex) == FF_TYPE_TEXT)
			result.PointerValue = (void*)pCurr->StrDefaultValue;
		else
			result.UIntValue = *(unsigned int*)&pCurr->DefaultValue;
	} else {
    result.UIntValue = FF_FAIL;
  }
	return result;
}

bool CFFGLPluginManager::GetTimeSupported() const
{
  return m_timeSupported;
}
