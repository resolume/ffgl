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
// www.harrisondigitalmedia.com
//
// Last modified: October 26 2006
//

#ifndef FFGLPLUGINSDK_STANDARD
#define FFGLPLUGINSDK_STANDARD

#include "FFGLPluginManager.h"
#include "FFGLPluginInfo.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class		CFFGLPlugin
///	\brief		CFFGLPlugin is the base class for all FreeFrameGL plugins developed with the FreeFrameGL SDK.
/// \author		Gualtiero Volpe
/// \version	1.0.0.2
///
/// The CFFGLPlugin class is the base class for every FreeFrameGL plugins developed with the FreeFrameGL SDK.
/// It is derived from CFFGLPluginManager, so that most of the plugin management and communication with the host
/// can be transparently handled through the default implementations of the methods of CFFGLPluginManager.
/// While CFFGLPluginManager is used by the global FreeFrame methods, CFFGLPlugin provides a default implementation
/// of the instance specific FreeFrame functions. Note that CFFGLPlugin methods are virtual methods: any given
/// FreeFrameGL plugin developed with the FreeFrameGL SDK will be a derived class of CFFGLPlugin and will have to
/// provide a custom implementation of most of such methods. Except for CFFGLPlugin::GetParameterDisplay and
/// CFFGLPlugin::GetInputStatus, all the default methods of CFFGLPlugin just return FF_FAIL: every derived
/// plugin is responsible of providing its specific implementation of such default methods.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CFFGLPlugin : public CFFGLPluginManager
{
public:
	/// The standard destructor of CFFGLPlugin.
	virtual ~CFFGLPlugin();

	/// Default implementation of the FFGL InitGL instance specific function. This function allocates
	/// the OpenGL resources the plugin needs during its lifetime
	///
	/// \param		vp Pointer to a FFGLViewportStruct structure (see the definition in FFGL.h and
	///						the description in the FFGL specification).
	/// \return		The default implementation always returns FF_SUCCESS.
	///						A custom implementation must be provided by every specific plugin that allocates
	///           any OpenGL resources
	virtual FFResult InitGL( const FFGLViewportStruct* vp )
	{
		currentViewport = *vp;
		return FF_SUCCESS;
	}

	/// Default implementation of the FFGL DeInitGL instance specific function. This function frees
	/// any OpenGL resources the plugin has allocated
	///
	/// \return		The default implementation always returns FF_SUCCESS.
	///						A custom implementation must be provided by every specific plugin that allocates
	///           any OpenGL resources
	virtual FFResult DeInitGL()
	{
		return FF_SUCCESS;
	}

	/// Default implementation of the FreeFrame getParameterDisplay instance specific function. It provides a string
	/// to display as the value of the plugin parameter whose index is passed as parameter to the method. This default
	/// implementation just returns the string representation of the float value of the plugin parameter. A custom
	/// implementation may be provided by every specific plugin.
	///
	/// \param		dwIndex		The index of the parameter whose display value is queried.
	///							It should be in the range [0, Number of plugin parameters).
	/// \return					The display value of the plugin parameter or NULL in case of error
	virtual char* GetParameterDisplay( unsigned int index );

	//Added the following to obscure the casting to FFMixed from implementations. Could/should also deal with other paramter types
	//in a similar way
	virtual FFResult SetFloatParameter( unsigned int index, float value );
	virtual FFResult SetTextParameter( unsigned int index, const char* value );
	virtual float GetFloatParameter( unsigned int index );
	virtual char* GetTextParameter( unsigned int index );

	void SetParamInfof( unsigned int index, const char* pchName, unsigned int type )
	{
		SetParamInfo( index, pchName, type, GetFloatParameter( index ) );
	}
	/// Default implementation of the FFGL ProcessOpenGL instance specific function. This function processes
	/// the input texture(s) by
	///
	/// \param		pOpenGLData to a ProcessOpenGLStruct structure (see the definition in FFGL.h and
	///						the description in the FFGL specification).
	/// \return		The default implementation always returns FF_FAIL.
	///						A custom implementation must be provided by every specific plugin.
	virtual FFResult ProcessOpenGL( ProcessOpenGLStruct* pOpenGLData )
	{
		return FF_FAIL;
	}

	/// Default implementation of the FFGL SetTime instance specific function
	///
	/// \param		pOpenGLData to a ProcessOpenGLStruct structure (see the definition in FFGL.h and
	///						the description in the FFGL specification).
	/// \return		The default implementation always returns FF_FAIL.
	///						A custom implementation must be provided by every specific plugin.
	virtual FFResult SetTime( double time )
	{
		hostTime = time;
		return FF_SUCCESS;
	}

	virtual void SetBeatInfo( float bpm, float barPhase );

	virtual void SetHostInfo(const char * hostname, const char * version);

	virtual void SetSampleRate(unsigned int sampleRate);

	/// Default implementation of the FreeFrame getInputStatus instance specific function. This function is called
	/// to know whether a given input is currently in use. For the default implementation every input is always in use.
	/// A custom implementation may be provided by every specific plugin.
	///
	/// \param		dwIndex		The index of the input whose status is queried.
	///							It should be in the range [Minimum number of inputs, Maximum number of inputs).
	/// \return					The default implementation always returns FF_FF_INPUT_INUSE or FF_FAIL if the index
	///							is out of range. A custom implementation may be provided by every specific plugin.
	virtual FFResult GetInputStatus( unsigned int index );

	////////////////////////////////////////////////////////////////////////////
	/// Additional FFGL functions by Resolume
	/// Returns the short name of the plugin
	virtual const char* GetShortName()
	{
		return 0;
	}

	/// Default implementation of the FFGL Connect instance specific function.
	/// Used to setup plugin when it is activated.
	/// Will get automatically called if necessary before ProcessGL
	/// \return		The default implementation always returns FF_SUCCESS.
	virtual unsigned int Connect()
	{
		return FF_SUCCESS;
	}

	/// Default implementation of the FFGL Disconnect instance specific function.
	/// Automatically gets called if necessary before DeInitGL
	/// \return		The default implementation always returns FF_SUCCESS.
	virtual unsigned int Disconnect()
	{
		return FF_SUCCESS;
	}

	/// Called when plugin viewport size is changed
	/// \param		vp Pointer to a FFGLViewportStruct structure (see the definition in FFGL.h and
	///				the description in the FFGL specification).
	/// \return		The default implementation always returns FF_SUCCESS.
	virtual unsigned int Resize( const FFGLViewportStruct* vp )
	{
		currentViewport = *vp;
		return FF_SUCCESS;
	}

	/// This flag indicates that Connect has been called by the host, or automatically called by FFGL
	bool m_isConnected;

	/// The only public data field CFFGLPlugin contains is m_pPlugin, a pointer to the plugin instance.
	/// Subclasses may use this pointer for self-referencing (e.g., a plugin may pass this pointer to external modules,
	/// so that they can use it for calling the plugin methods).
	CFFGLPlugin* m_pPlugin;

protected:
	/// The only protected function of CFFGLPlugin is its constructor. In fact, nor CFFGLPluginManager objects nor
	/// CFFGLPlugin objects should be created directly, but only objects of the subclasses implementing specific
	/// plugins should be instantiated. Moreover, subclasses should define and provide a factory method to be used by
	/// the FreeFrame SDK for instantiating plugin objects.
	CFFGLPlugin( bool supportTopLeftTextureOrientation = false );

	FFGLViewportStruct currentViewport;
	float bpm;
	float barPhase;
	
	struct HostInfo {
		std::string name;
		std::string version;
	} hostInfos;
	double hostTime;
	int sampleRate;
};

#endif
