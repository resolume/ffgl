#pragma once

#include <FFGLShader.h>
#include "FFGLPluginSDK.h"
#include <string>

class AddBlending : public CFreeFrameGLPlugin
{
public:
	AddBlending();
	~AddBlending();

	///////////////////////////////////////////////////
	// FreeFrame plugin methods
	///////////////////////////////////////////////////
	
	FFResult SetFloatParameter(unsigned int dwIndex, float value) override;		
	float GetFloatParameter(unsigned int index) override;					
	FFResult ProcessOpenGL(ProcessOpenGLStruct* pGL) override;
	FFResult InitGL(const FFGLViewportStruct *vp);
	FFResult DeInitGL();

	//The mixer short name
	const char * GetShortName() { static const char* sname = "AddB"; return sname; }

	///////////////////////////////////////////////////
	// Factory method
	///////////////////////////////////////////////////

	static FFResult __stdcall CreateInstance(CFreeFrameGLPlugin **ppOutInstance)
  {
	  *ppOutInstance = new AddBlending();
	  if (*ppOutInstance != NULL)
		  return FF_SUCCESS;
	  return FF_FAIL;
  }


protected:	
	// Parameters
	float m_MixVal;
	int m_initResources;

    FFGLShader m_shader;
	GLint m_inputTextureLocation1;
	GLint m_inputTextureLocation2;
	GLint m_MixValLocation;

};
