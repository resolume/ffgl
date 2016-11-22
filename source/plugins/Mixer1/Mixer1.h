#pragma once

#include <FFGLShader.h>
#include "FFGLPluginSDK.h"
#include <string>

class Mixer1 : public CFreeFrameGLPlugin
{
public:
	Mixer1();
	~Mixer1();

	///////////////////////////////////////////////////
	// FreeFrame plugin methods
	///////////////////////////////////////////////////
	
	FFResult SetFloatParameter(unsigned int dwIndex, float value) override;		
	float GetFloatParameter(unsigned int index) override;					
	FFResult ProcessOpenGL(ProcessOpenGLStruct* pGL) override;
	FFResult InitGL(const FFGLViewportStruct *vp);
	FFResult DeInitGL();

	const char * GetShortName() { static const char* sname = "MX01"; return sname; }

	///////////////////////////////////////////////////
	// Factory method
	///////////////////////////////////////////////////

	static FFResult __stdcall CreateInstance(CFreeFrameGLPlugin **ppOutInstance)
  {
	  *ppOutInstance = new Mixer1();
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
