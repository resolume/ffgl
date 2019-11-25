#define _CRT_SECURE_NO_WARNINGS
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
// Developer: Trey Harrison
// mailto:trey@treyharrison.com
//
// Last modified: Oct. 26 2006
//

#include "FFGLPluginSDK.h"
#include <stdio.h>
#include <memory.h>
#include <algorithm>

// Buffer used by the default implementation of getParameterDisplay
static char s_DisplayValue[ 16 ];

////////////////////////////////////////////////////////
// CFFGLPlugin constructor and destructor
////////////////////////////////////////////////////////

CFFGLPlugin::CFFGLPlugin( bool supportTopLeftTextureOrientation ) :
	CFFGLPluginManager( supportTopLeftTextureOrientation ),
	bpm( 120.0f ),
	barPhase( 0.0f )
{
}

CFFGLPlugin::~CFFGLPlugin()
{
}

////////////////////////////////////////////////////////
// Default implementation of CFFGLPlugin methods
////////////////////////////////////////////////////////

char* CFFGLPlugin::GetParameterDisplay( unsigned int index )
{
	unsigned int pType = m_pPlugin->GetParamType( index );
	if( pType != FF_FAIL )
	{
		if( pType == FF_TYPE_TEXT || pType == FF_TYPE_FILE )
		{
			return m_pPlugin->GetTextParameter( index );
		}
		else
		{
			std::string stringValue = std::to_string(m_pPlugin->GetFloatParameter( index ));
			memset(s_DisplayValue, 0, sizeof(s_DisplayValue));
			memcpy(s_DisplayValue, stringValue.c_str(), std::min(sizeof(s_DisplayValue), stringValue.length()));
			return s_DisplayValue;
		}
	}
	return NULL;
}

FFResult CFFGLPlugin::SetFloatParameter( unsigned int index, float value )
{
	return FF_FAIL;
}

FFResult CFFGLPlugin::SetTextParameter( unsigned int index, const char* value )
{
	return FF_FAIL;
}

float CFFGLPlugin::GetFloatParameter( unsigned int index )
{
	return 0.0;
}

char* CFFGLPlugin::GetTextParameter( unsigned int index )
{
	return (char*)FF_FAIL;
}

FFResult CFFGLPlugin::GetInputStatus( unsigned int index )
{
	if( index >= GetMaxInputs() )
		return FF_FAIL;
	return FF_INPUT_INUSE;
}

void CFFGLPlugin::SetBeatInfo( float bpm, float barPhase )
{
	this->bpm = bpm;
	this->barPhase = barPhase;
}

void CFFGLPlugin::SetHostInfo(const char * hostname, const char * version)
{
	hostInfos.name = hostname;
	hostInfos.version = version;
}

void CFFGLPlugin::SetSampleRate(unsigned int _sampleRate)
{
	sampleRate = _sampleRate;
}

