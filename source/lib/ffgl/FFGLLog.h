#pragma once
#include "FFGL.h"

class FFGLLog
{
public:
	static void SetLogCallback( PFNLog logCallback );
	static void LogToHost( const char* cStr );
};