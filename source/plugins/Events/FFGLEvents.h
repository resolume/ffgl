#pragma once
#include <FFGLSDK.h>

class FFGLEvents : public CFFGLPlugin
{
public:
	FFGLEvents();

	//CFFGLPlugin
	FFResult ProcessOpenGL( ProcessOpenGLStruct* pGL ) override;

	FFResult SetFloatParameter( unsigned int dwIndex, float value ) override;
	FFResult SetTextParameter( unsigned int index, const char* value ) override;

	float GetFloatParameter( unsigned int index ) override;
	char* GetTextParameter( unsigned int index ) override;

private:
	void Randomize();

	float floatParam;
	float optionParam;
	std::string textParam;
	float rgba[ 4 ];
	float hsba[ 4 ];
};
