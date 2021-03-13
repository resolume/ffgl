#include "FFGLLog.h"

static PFNLog s_logCallback = nullptr;

void FFGLLog::SetLogCallback( PFNLog logCallback )
{
	s_logCallback = logCallback;
}
void FFGLLog::LogToHost( const char* cStr )
{
	if( s_logCallback != nullptr && cStr != nullptr )
		s_logCallback( const_cast< char* >( cStr ) );
}