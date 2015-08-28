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
// Copyright (c) 2002, 2003, 2004, 2006 www.freeframe.org
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "FFGLPluginSDK.h"
#include <memory.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Static and extern variables used in the FreeFrame SDK 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern CFFGLPluginInfo* g_CurrPluginInfo;

static CFreeFrameGLPlugin* s_pPrototype = NULL;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FreeFrame SDK default implementation of the FreeFrame global functions. 
// Such function are called by the plugMain function, the only function a plugin exposes.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void *getInfo() 
{
	return (void *)(g_CurrPluginInfo->GetPluginInfo());
}

FFResult initialise()
{
  if (g_CurrPluginInfo==NULL)
    return FF_FAIL;

	if (s_pPrototype==NULL)
  {
    //get the instantiate function pointer
    FPCREATEINSTANCEGL *pInstantiate = g_CurrPluginInfo->GetFactoryMethod();

	  //call the instantiate function
    FFResult ret = pInstantiate(&s_pPrototype);

    //make sure the instantiate call worked
    if ((ret == FF_FAIL) || (s_pPrototype == NULL))
      return FF_FAIL;

    return FF_SUCCESS;
	}

	return FF_SUCCESS; 
}

FFResult deInitialise()
{
	if (s_pPrototype != NULL) {
		delete s_pPrototype;
		s_pPrototype = NULL;
	}
	return FF_SUCCESS;
}

unsigned int getNumParameters() 
{
	if (s_pPrototype == NULL) {
		FFResult dwRet = initialise();
		if (dwRet == FF_FAIL) return FF_FAIL;
	}

	return s_pPrototype->GetNumParams();
}
							
char* getParameterName(unsigned int index)
{
	if (s_pPrototype == NULL) {
		FFResult dwRet = initialise();
		if (dwRet == FF_FAIL) return NULL;
	}
	
	return s_pPrototype->GetParamName(index);
}

FFMixed getParameterDefault(unsigned int index)
{
  FFMixed ret;
  ret.UIntValue = FF_FAIL;
	if (s_pPrototype == NULL) {
		FFResult dwRet = initialise();
		if (dwRet == FF_FAIL) return ret;
	}
	return s_pPrototype->GetParamDefault(index);
}

FFResult getPluginCaps(unsigned int index)
{
	int MinInputs = -1;
	int MaxInputs = -1;

	if (s_pPrototype == NULL) {
		FFResult dwRet = initialise();
		if (dwRet == FF_FAIL) return FF_FAIL;
	}

	switch (index) {

	case FF_CAP_16BITVIDEO:
		return FF_FALSE;

	case FF_CAP_24BITVIDEO:
		return FF_FALSE;

	case FF_CAP_32BITVIDEO:
		return FF_FALSE;

	case FF_CAP_PROCESSFRAMECOPY:
		return FF_FALSE;

  case FF_CAP_PROCESSOPENGL:
		return FF_TRUE;

  case FF_CAP_SETTIME:
    if (s_pPrototype->GetTimeSupported())
      return FF_TRUE;
    else
      return FF_FALSE;

	case FF_CAP_MINIMUMINPUTFRAMES:
		MinInputs = s_pPrototype->GetMinInputs();
		if (MinInputs < 0) return FF_FALSE;
		return MinInputs;

	case FF_CAP_MAXIMUMINPUTFRAMES:
		MaxInputs = s_pPrototype->GetMaxInputs();
		if (MaxInputs < 0) return FF_FALSE;
		return MaxInputs;

	case FF_CAP_COPYORINPLACE:
  	return FF_FALSE;

	default:
		return FF_FALSE;
	}
	
	return FF_FAIL;
}

void *getExtendedInfo()
{
	return (void *)(g_CurrPluginInfo->GetPluginExtendedInfo());
}

unsigned int getParameterType(unsigned int index)
{
	if (s_pPrototype == NULL) {
		FFResult dwRet = initialise();
		if (dwRet == FF_FAIL) return FF_FAIL;
	}
	
	return s_pPrototype->GetParamType(index);
}


#ifdef FFGL_EXT

FFUInt32 getNumParameterElements(unsigned int index)
{
	if (s_pPrototype == NULL) {
		FFResult dwRet = initialise();
		if (dwRet == FF_FAIL) return FF_FAIL;
	}
	
	return s_pPrototype->GetNumParamElements(index);
}

FFUInt32 getParameterUsage(unsigned int index)
{
	if (s_pPrototype == NULL)
    {
		FFResult dwRet = initialise();
		if (dwRet == FF_FAIL)
            return FF_FAIL;
	}
	
	return s_pPrototype->GetParamUsage(index);
}

const char* getPluginShortName()
{
    if (s_pPrototype == NULL)
    {
        FFResult dwRet = initialise();
        if (dwRet == FF_FAIL)
            return NULL;
    }

    const char * shortName = s_pPrototype->GetShortName();
    if(shortName == NULL)
        return NULL;

    return shortName;
}


#endif



void *instantiateGL(const FFGLViewportStruct *pGLViewport)
{
	if (g_CurrPluginInfo==NULL || pGLViewport==NULL)
    return (void *)FF_FAIL;

  // If the plugin is not initialized, initialize it
  if (s_pPrototype == NULL)
  {
		FFResult dwRet = initialise();

	  if ((dwRet == FF_FAIL) || (s_pPrototype == NULL))
      return (void *)FF_FAIL;
	}
		
	//get the instantiate function pointer
  FPCREATEINSTANCEGL *pInstantiate = g_CurrPluginInfo->GetFactoryMethod();

	CFreeFrameGLPlugin *pInstance = NULL;

  //call the instantiate function
  FFResult dwRet = pInstantiate(&pInstance);

  //make sure the instantiate call worked
  if ((dwRet == FF_FAIL) || (pInstance == NULL))
    return (void *)FF_FAIL;

	pInstance->m_pPlugin = pInstance;
		
	// Initializing instance with default values
	for (unsigned int i = 0; i < s_pPrototype->GetNumParams(); ++i)
  {
		unsigned int pType = s_pPrototype->GetParamType(i);
		FFMixed pDefault = s_pPrototype->GetParamDefault(i);
    if (pType == FF_TYPE_TEXT)
      dwRet = pInstance->SetTextParameter(i, (const char *)pDefault.PointerValue);
    else
      dwRet = pInstance->SetFloatParameter(i, *(float *)&pDefault.UIntValue);


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
//#ifdef FFGL_EXT
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
//#endif
//		default:
//         memcpy(&ParamStruct.NewParameterValue, pValue, 4);
//         dwRet = pInstance->SetParameter(&ParamStruct);
//         break;
//         
//    }

		if (dwRet == FF_FAIL)
    {
      //SetParameter failed, delete the instance
      delete pInstance;
      return (void *)FF_FAIL;
    }
	}

	//call the InitGL method
  if (pInstance->InitGL(pGLViewport)==FF_SUCCESS)
  {
    //succes? we're done.
    return pInstance;
  }

  //InitGL failed, delete the instance
  pInstance->DeInitGL();
  delete pInstance;

  return (void *)FF_FAIL;
}

FFResult deInstantiateGL(void *instanceID)
{
  CFreeFrameGLPlugin *p = (CFreeFrameGLPlugin *)instanceID;

  if (p != NULL)
  {
    // Disconnect if necessary
	if(p->m_isConnected)
	{
		p->Disconnect();
		p->m_isConnected = false;
	}

    p->DeInitGL();
		delete p;

		return FF_SUCCESS;
	}

	return FF_FAIL;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implementation of plugMain, the one and only exposed function
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32

   FFMixed __stdcall plugMain(FFUInt32 functionCode, FFMixed inputValue, FFInstanceID instanceID) 

#elif TARGET_OS_MAC

   FFMixed plugMain(FFUInt32 functionCode, FFMixed inputValue, FFInstanceID instanceID) 

#elif __linux__

   FFMixed plugMain(FFUInt32 functionCode, FFMixed inputValue, FFInstanceID instanceID)

#endif	

{
	FFMixed retval;

	// declare pPlugObj - pointer to this instance
	CFreeFrameGLPlugin* pPlugObj;

	// typecast DWORD into pointer to a CFreeFrameGLPlugin
	pPlugObj = (CFreeFrameGLPlugin*) instanceID;

	switch (functionCode) {

	case FF_GETINFO:
		retval.PointerValue = (PluginInfoStruct*)getInfo();
		break;

	case FF_INITIALISE:
		retval.UIntValue = initialise();
		break;

	case FF_DEINITIALISE:
		retval.UIntValue = deInitialise();	
		break;

	case FF_GETNUMPARAMETERS:
		retval.UIntValue = getNumParameters();
		break;

	case FF_GETPARAMETERNAME:
		retval.PointerValue = getParameterName(inputValue.UIntValue);
		break;
	
	case FF_GETPARAMETERDEFAULT:
		retval = getParameterDefault(inputValue.UIntValue);
		break;

	case FF_GETPLUGINCAPS:
		retval.UIntValue = getPluginCaps(inputValue.UIntValue);
		break;

	case FF_GETEXTENDEDINFO: 
		retval.PointerValue = getExtendedInfo();
		break;

	case FF_GETPARAMETERTYPE:		
		retval.UIntValue = getParameterType(inputValue.UIntValue);
		break;

	case FF_GETPARAMETERDISPLAY:
		if (pPlugObj != NULL) 
			retval.PointerValue = pPlugObj->GetParameterDisplay(inputValue.UIntValue);
		else
			retval.PointerValue = (char*)FF_FAIL;
		break;
		
	case FF_SETPARAMETER:
		if (pPlugObj != NULL)
		{
			if (getParameterType(((const SetParameterStruct*)inputValue.PointerValue)->ParameterNumber) == FF_TYPE_TEXT)
				retval.UIntValue = pPlugObj->SetTextParameter(((const SetParameterStruct*)inputValue.PointerValue)->ParameterNumber,
															  (const char *)((const SetParameterStruct*)inputValue.PointerValue)->NewParameterValue.PointerValue);
			else
				retval.UIntValue = pPlugObj->SetFloatParameter(((const SetParameterStruct*)inputValue.PointerValue)->ParameterNumber,
															   (*(float *)&((const SetParameterStruct*)inputValue.PointerValue)->NewParameterValue.UIntValue));
		} else {
			retval.UIntValue = FF_FAIL;
		}
		break;
	
	case FF_GETPARAMETER:
		if (pPlugObj != NULL) {
      if (getParameterType(inputValue.UIntValue) == FF_TYPE_TEXT)
        retval.PointerValue = pPlugObj->GetTextParameter(inputValue.UIntValue);
	  else {
		float fValue = pPlugObj->GetFloatParameter(inputValue.UIntValue);
		retval.UIntValue = *(FFUInt32 *)&fValue;
	  }
        
    }
			
		else 
			retval.UIntValue = FF_FAIL;
		break;
		
  case FF_INSTANTIATEGL:
    retval.PointerValue = instantiateGL((const FFGLViewportStruct *)inputValue.PointerValue);
    break;

  case FF_DEINSTANTIATEGL:
    if (pPlugObj != NULL)
			retval.UIntValue = deInstantiateGL(pPlugObj);
		else
			retval.UIntValue = FF_FAIL;
    break;
	
	case FF_GETINPUTSTATUS:
		if (pPlugObj != NULL)
			retval.UIntValue = pPlugObj->GetInputStatus(inputValue.UIntValue);
		else
			retval.UIntValue = FF_FAIL;
		break;

  case FF_PROCESSOPENGL:
    if (pPlugObj != NULL)
    {
      ProcessOpenGLStruct *pogls = (ProcessOpenGLStruct *)inputValue.PointerValue;
      if (pogls!=NULL)
	  {
		// make sure Connect has been called
		if(!pPlugObj->m_isConnected)
		{
			pPlugObj->Connect();
			pPlugObj->m_isConnected = true;
		}

        retval.UIntValue = pPlugObj->ProcessOpenGL(pogls);
	  }
      else
        retval.UIntValue = FF_FAIL;
    }
		else
			retval.UIntValue = FF_FAIL;
		break;

  #ifdef FFGL_EXT
  case FF_GETNUMPARAMETERELEMENTS:		
	  retval.UIntValue = getNumParameterElements(inputValue.UIntValue);
    break;
    
  case FF_GETPARAMETERUSAGE:		
	  retval.UIntValue = getParameterUsage(inputValue.UIntValue);
    break;
    
  case FF_GETPLUGINSHORTNAME:
	  retval.PointerValue = (void*)getPluginShortName();
      break;

	
			
#endif

  case FF_SETTIME:
    if (pPlugObj != NULL)
    {
      double *inputTime = (double *)inputValue.PointerValue;
      if (inputTime!=NULL)
        retval.UIntValue = pPlugObj->SetTime(*inputTime);
      else
        retval.UIntValue = FF_FAIL;
    }
		else
			retval.UIntValue = FF_FAIL;
		break;

    //////////////////////////////////////////////////
	// Custom Resolume plugin functions
	case FF_CONNECT:
		if (pPlugObj != NULL)
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
		if (pPlugObj != NULL)
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
		if (pPlugObj != NULL)
		{
			retval.UIntValue = pPlugObj->Resize( (const FFGLViewportStruct *)inputValue.PointerValue );
		}
		else
		{
			retval.UIntValue = FF_FAIL;
		}
		break;

  //these old FF functions must always fail for FFGL plugins
	case FF_INSTANTIATE:
  case FF_DEINSTANTIATE:
	case FF_PROCESSFRAME:
  case FF_PROCESSFRAMECOPY:
	default:
		retval.UIntValue = FF_FAIL;
		break;
	}
	
	return retval;
}
