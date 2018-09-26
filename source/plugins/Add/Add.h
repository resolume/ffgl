#pragma once
#include <FFGLSDK.h>
#include <string>

class Add : public CFreeFrameGLPlugin
{
public:
	Add();
	~Add();

	///////////////////////////////////////////////////
	// FreeFrame plugin methods
	///////////////////////////////////////////////////

	FFResult SetFloatParameter( unsigned int dwIndex, float value ) override;
	float GetFloatParameter( unsigned int index ) override;
	FFResult ProcessOpenGL( ProcessOpenGLStruct* pGL ) override;
	FFResult InitGL( const FFGLViewportStruct* vp ) override;
	FFResult DeInitGL() override;

	//The mixer short name
	const char* GetShortName() override
	{
		static const char* sname = "AddB";
		return sname;
	}

	///////////////////////////////////////////////////
	// Factory method
	///////////////////////////////////////////////////

	static FFResult __stdcall CreateInstance( CFreeFrameGLPlugin** ppOutInstance )
	{
		*ppOutInstance = new Add();
		if( *ppOutInstance != NULL )
			return FF_SUCCESS;
		return FF_FAIL;
	}

protected:
	// Parameters
	float m_MixVal;

	ffglex::FFGLShader shader;
	ffglex::FFGLScreenQuad quad;
	GLint mixValLocation;
	GLint maxUVDestLocation;
	GLint maxUVSrcLocation;
};
