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

// Buffer used by the default implementation of getParameterDisplay
static char s_DisplayValue[5];


////////////////////////////////////////////////////////
// CFreeFrameGLPlugin constructor and destructor
////////////////////////////////////////////////////////

CFreeFrameGLPlugin::CFreeFrameGLPlugin()
: CFFGLPluginManager()
{
}

CFreeFrameGLPlugin::~CFreeFrameGLPlugin() 
{
}


////////////////////////////////////////////////////////
// Default implementation of CFreeFrameGLPlugin methods
////////////////////////////////////////////////////////

char* CFreeFrameGLPlugin::GetParameterDisplay(unsigned int index) 
{	
	unsigned int pType = m_pPlugin->GetParamType(index);

	if (pType != FF_FAIL)
  {
		if (pType == FF_TYPE_TEXT)
    {
			return m_pPlugin->GetTextParameter(index);
    }
		else
    {
			float fValue = m_pPlugin->GetFloatParameter(index);
			memset(s_DisplayValue, 0, 5);
			sprintf(s_DisplayValue, "%f", fValue);
			return s_DisplayValue;
		}
	}
	return NULL;
}			

FFResult CFreeFrameGLPlugin::SetFloatParameter(unsigned int index, float value)
{
  return FF_FAIL;
}

FFResult CFreeFrameGLPlugin::SetTextParameter(unsigned int index, const char *value)
{
  return FF_FAIL;
}

float CFreeFrameGLPlugin::GetFloatParameter(unsigned int index)
{
  return 0.0;
}

char* CFreeFrameGLPlugin::GetTextParameter(unsigned int index)
{
  return (char *)FF_FAIL;
}					

FFResult CFreeFrameGLPlugin::GetInputStatus(unsigned int index)
{
	if (index >= GetMaxInputs()) return FF_FAIL;
	return FF_INPUT_INUSE;
}
