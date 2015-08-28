#ifndef AVFFGLAddSubtract_H
#define AVFFGLAddSubtract_H

#include <FFGLShader.h>
#include "FFGLPluginSDK.h"

class AVFFGLAddSubtract :
public CFreeFrameGLPlugin
{
public:
	AVFFGLAddSubtract();
  ~AVFFGLAddSubtract() {}

	///////////////////////////////////////////////////
	// FreeFrame plugin methods
	///////////////////////////////////////////////////
	
	FFResult SetFloatParameter(unsigned int dwIndex, float value) override;		
	float GetFloatParameter(unsigned int index) override;					
	FFResult ProcessOpenGL(ProcessOpenGLStruct* pGL) override;
	FFResult InitGL(const FFGLViewportStruct *vp);
	FFResult DeInitGL();

	///////////////////////////////////////////////////
	// Factory method
	///////////////////////////////////////////////////

	static FFResult __stdcall CreateInstance(CFreeFrameGLPlugin **ppOutInstance)
  {
  	*ppOutInstance = new AVFFGLAddSubtract();
	  if (*ppOutInstance != NULL)
      return FF_SUCCESS;
	  return FF_FAIL;
  }


protected:	
	// Parameters
	float m_BrightnessR;
	float m_BrightnessG;
	float m_BrightnessB;

	int m_initResources;

	FFGLExtensions m_extensions;
    FFGLShader m_shader;
	GLint m_inputTextureLocation;
	GLint m_BrightnessLocation;
	//GLint m_BrightnessGLocation;
	//GLint m_BrightnessBLocation;

};


#endif
