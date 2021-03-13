#pragma once

#if defined( _WIN32 ) || defined( _WIN64 )
#	define FFGL_WINDOWS
#elif defined( __linux__ )
#	define FFGL_LINUX
#elif defined( __APPLE__ )
#	include <TargetConditionals.h>
#	include <AvailabilityMacros.h>
#	if TARGET_OS_MAC
#		define FFGL_MACOS
#	else
#		error "We dont support building for anything other than a mac"
#	endif
#else
#	error "No Platform defined"
#endif

#if defined( _DEBUG ) || defined( DEBUG )
#	define FFGL_DEBUG
#endif
