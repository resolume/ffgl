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

#ifndef FFGLPLUGINMANAGER_STANDARD
#define FFGLPLUGINMANAGER_STANDARD
#include <vector>
#include <string>

#include "FFGL.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class		CFFGLPluginManager
///	\brief		CFFGLPluginManager manages information concerning a plugin inputs, parameters, and capabilities.
/// \authors	Gualtiero Volpe, Menno Vink (menno@resolume.com)
/// \version	2.0.0
///
/// The CFFGLPluginManager class is the base class for FreeFrameGL plugins developed with the FreeFrameGL SDK since it provides
/// them with methods for automatically manage information concerning plugin inputs, paramaters, and capabilities.
/// Examples of information managed by this class are the number of inputs and parameters of a plugin; the name, type and
/// default value of each parameter.
/// Plugins developed with the FFGL SDK (and thus having this class as base class) should call the protected methods
/// of this class in order to specify the information related to their inputs and parameters. These calls
/// are usually done while constructing the plugin subclass. Plugins subclasses should also call methods of this class in
/// order to get information about the images they are going to process (i.e., their width and height).
/// The default implementations of the FFGL global functions call the public methods of this class in order to
/// return to the host information about a plugin.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CFFGLPluginManager
{
public:
	/// The standard destructor of CFFGLPluginManager.
	virtual ~CFFGLPluginManager();

	/// This method returns the minimum number of inputs the host must provide.
	/// It is usually called by the default implementations of the FreeFrame global functions.
	///
	/// \return		The minimum number of inputs the host must provide.
	unsigned int GetMinInputs() const;
	/// This method returns the maximum number of inputs the plugin can receive.
	/// It is usually called by the default implementations of the FreeFrame global functions.
	///
	/// \return		The maximum number of inputs the plugin can receive.
	unsigned int GetMaxInputs() const;

	/// This method is called by the host to determine whether the plugin supports the SetTime function.
	bool IsTimeSupported() const;
	/// This methos is called by the host to determine whether or not the plugin supports top left texture orientations.
	/// Some hosts may use top-left texture orientations internally. By default plugins use bottom left. If the host wants
	/// to call those plugins it first needs to flip the inputs on the y-axis, and then when the plugin has run the host needs
	/// to flip the output as well. This takes some time, so if the plugin reports that it supports top-left orientations the
	/// host may do this so that it doesn't have to do the texture flipping.
	bool IsTopLeftTextureOrientationSupported() const;
	/// This methos is called by the host to tell the plugin that the host will be providing textures oriented at the top-left.
	/// The host is only allowed to do this if this plugin supports the top-left orientation. This function just toggles the internal
	/// textureOrientation variable so that the plugin implementation may query it and see that it can use the top-left orientation.
	/// There's no guarantee that a host will actually ever call this. Hosts that are using bottom-left orientation internally will probably
	/// never even query the support, let alone enable it.
	void HostEnabledTopLeftTextures();

	/// This method returns how may parameters the plugin has.
	/// It is usually called by the default implementations of the FreeFrame global functions.
	///
	/// \return		The number of parameters of the plugin.
	unsigned int GetNumParams() const;
	/// This method returns the name of the plugin parameter whose index is passed as parameter
	/// to the method. It is usually called by the default implementations of the FreeFrame global functions.
	///
	/// \param	dwIndex		The index of the plugin parameter whose name is queried.
	///						It should be in the range [0, Number of plugin parameters).
	/// \return				The name of the plugin parameter whose index is passed to the method.
	///						The return value is a pointer to an array of 16 1-byte ASCII characters,
	///						not null terminated (see FreeFrame specification). NULL is returned on error.
	char* GetParamName( unsigned int index );
	/// This method is called to know the type of the plugin parameter whose index is passed as parameter
	/// to the method. It is usually called by the default implementations of the FreeFrame global functions.
	///
	/// \param	dwIndex		The index of the plugin parameter whose name is queried.
	///						It should be in the range [0, Number of plugin parameters).
	/// \return				The type of the plugin parameter whose index is passed as parameter to the method.
	///						Codes for allowed parameter types are defined in FreeFrame.h.
	///						In case of error, FF_FAIL is returned.
	unsigned int GetParamType( unsigned int index ) const;
	/// This method is called to know the usage of a plugin parameter
	/// For example a float buffer that expects a spectrum, will return FF_USAGE_FFT
	/// By default parameters will return 0 as FF_USAGE_STANDARD.
	/// In case parameter is an array, it will return the number of elements in the array.
	/// \param	dwIndex		The index of the plugin parameter whose name is queried.
	///						It should be in the range [0, Number of plugin parameters).
	/// \return				The type of the plugin parameter whose index is passed as parameter to the method.
	///						Codes for allowed parameter types are defined in FreeFrame.h.
	///						In case of error, FF_FAIL is returned.
	unsigned int GetParamUsage( unsigned int dwIndex ) const;
	/// This method is called to get the default value of the plugin parameter whose index is passed as parameter
	/// to the method. It is usually called by the default implementations of the FreeFrame global functions.
	///
	/// \param	dwIndex		The index of the plugin parameter whose name is queried.
	///						It should be in the range [0, Number of plugin parameters).
	/// \return				The default value of the plugin parameter whose index is passed as parameter to the method.
	///						The return value should be cast either to a char* in case of text parameters or to a float*
	///						in any other case. In case of error, NULL is returned.
	FFMixed GetParamDefault( unsigned int dwIndex ) const;
	/// Get the visibility status of a certain parameter.
	///
	/// \param dwIndex		The index of the plugin parameter whose visibility state is queried.
	///						It should be in the range [0, Number of plugin parameters).
	/// \return				This returns 1 if the parameter is supposed to be visible or 0 when it needs to be invisible.
	///						If the parameter at dwIndex doesn't exist this returns FF_FAIL
	FFUInt32 GetParamVisibility( unsigned int dwIndex ) const;

	/// This method is called to know the number of elements of the plugin parameter whose index is passed as parameter
	/// to the method. It is usually called by the default implementations of the FreeFrame global functions.
	/// By default parameters will return 1.
	/// In case parameter is an array, it will return the number of elements in the array.
	/// \param	dwIndex		The index of the plugin parameter whose name is queried.
	///						It should be in the range [0, Number of plugin parameters).
	/// \return				The type of the plugin parameter whose index is passed as parameter to the method.
	///						Codes for allowed parameter types are defined in FreeFrame.h.
	///						In case of error, FF_FAIL is returned.
	unsigned int GetNumParamElements( unsigned int dwIndex ) const;
	/// Get the name of an element for a parameter that can have elements (eg FF_TYPE_OPTION and FF_TYPE_BUFFER)
	/// The host may use this name to display that element on the screen. For example an OPTION's element
	/// will most likely be show as a dropdown option, which when pressed will cause the param's value to change
	/// to that element's default value.
	///
	/// \param	dwIndex		The index of the plugin parameter which contains the element for which
	///						the name is being queried.
	///						It should be in the range[0, Number of plugin parameters).
	///	\param	elIndex		The index of the element whose name is queried.
	///						It should be in the range [0, parameter.Number of elements).
	///	\return				The name of the parameter's element whose indices are passed to the method.
	///						The return value is a pointer to a null terminated c string of ASCII characters.
	///						In case of error, NULL is returned.
	char* GetParamElementName( unsigned int dwIndex, unsigned int elIndex );
	/// Get the default value of an element for a parameter whose elements default values have any effect (eg FF_TYPE_OPTION)
	/// The host will use this value to set the param to this specific value when that option is chosen.
	///
	/// \param	dwIndex		The index of the plugin parameter which contains the element for which
	///						the default value is being queried.
	///						It should be in the range[0, Number of plugin parameters).
	///	\param	elIndex		The index of the element whose default value is queried.
	///						It should be in the range [0, parameter.Number of elements).
	///	\return				The default value of the parameter's element whose indices are passed to the method.
	///						The return value should be cast either to a float*.
	///						In case of error, NULL is returned.
	FFMixed GetParamElementDefault( unsigned int dwIndex, unsigned int elIndex ) const;
	/// Set the value of an element for a parameter whose elements values can change (eg FF_TYPE_BUFFER)
	/// The host will use this to change the contents of a parameter's elements, which the plugin can then use
	/// depending on what the buffer is used for.
	///
	/// \param	dwIndex		The index of the plugin parameter which contains the element for which
	///						the value is to be changed.
	///						It should be in the range[0, Number of plugin parameters).
	///	\param	elIndex		The index of the element whose value is to be  changed.
	///						It should be in the range [0, parameter.Number of elements).
	///	\return				FFGL result indicating if setting the value succeeded. Setting a value might fail
	///						if either of the provided indices is out of range. FF_SUCCESS on success, FF_FAIL otherwise.
	FFUInt32 SetParamElementValue( unsigned int dwIndex, unsigned int elIndex, float newValue );
	/// Get the number of element separators a parameter may have. Calling this only makes sense for FF_TYPE_OPTION parameters
	/// as those are the only parameters which can contain separatable elements.
	///
	/// \param	dwIndex		The index of the plugin parameter fir which you want to get the number of separators.
	/// \return				The number of separators in this parameter. This returns 0 if the parameter doesn't exist, doesn't support
	///						separators or just if it doesn't happen to have any.
	FFUInt32 GetNumElementSeparators( unsigned int dwIndex );
	/// Get the index of the element before which a paremeter's separator should be shown.
	///
	/// \param dwIndex		The index of the plugin parameter from which to get the separator's element index.
	///						It should be in the range [0, Number of plugin parameters).
	/// \separatorIndex		The index of the separator for which to get the element index.
	///						It should be in the range [0, GetNumElementSeparators).
	/// \return				The element index before which a separator should be shown. This will return -1 in case of
	///						an error (eg dwIndex or separatorIndex out of range).
	FFUInt32 GetElementSeparatorElementIndex( unsigned int dwIndex, unsigned int separatorIndex );

	unsigned int GetNumFileParamExtensions( unsigned int index ) const;
	char* GetFileParamExtension( unsigned int paramIndex, unsigned int extensionIndex ) const;

	RangeStruct GetParamRange( unsigned int index );

	/// Get the number of parameter events that are currently pending.
	FFUInt32 GetNumPendingParamEvents() const;
	/// Consumes currently pending parameter events.
	/// This function loops over the currently pending parameter events and writes out ParamEventStructs into the passed in events buffer.
	/// It then resets the events that it has consumed so that the next consume call will not return the same events.
	///
	/// \param events		The buffer into which param events should be written.
	/// \param maxNumEvents	The maximum number of events that should be consumed.
	/// \return				The number of events that were consumed. This may be smaller than maxNumEvents when there aren't any more pending events.
	FFUInt32 ConsumeParamEvents( ParamEventStruct* events, FFUInt32 maxNumEvents );

protected:
	/// The standard constructor of CFFGLPluginManager.
	///
	/// \param	supportTopLeftTextureOrientation		Enables support for textures with the top-left orientation. These textures have the left top-most texel at the uv
	///													coordinate 0, 0. Some hosts use top-left orientation internally, for those hosts it's faster if the plugin also uses the
	///													top-left orientation as that prevents the host from having to flip inputs and outputs on the y axis.
	///													Note that this flag only request's the support, usage of the orientation is not guaranteed. If the host intends to use
	///													top-left orientations it first checks if the plugin supports it, and when it does it tells the plugin that it will be using it.
	///													So in the implementation of your plugin you should access the textureOrientation variable to see if the host has enabled it.
	///													If the host hasn't enabled it you must make sure you match the host's texture orientation which is bottom-left for uv coordinate 0, 0.
	///
	/// \remark	Notice that the CFFGLPluginManager constructor is a protected member function, i.e., nor CFFGLPluginManager
	///		objects nor CFreeFramePlugin objects should be created directly, but only objects of the subclasses
	///			implementing specific plugins should be instantiated.
	CFFGLPluginManager( bool supportTopLeftTextureOrientation = false );

	/// This method is called by a plugin subclass, derived from this class, to indicate the minimum number
	/// of inputs the host must provide. This method is usually called when a plugin object is instantiated
	/// (i.e., in the plugin subclass constructor).
	///
	/// \param	iMinInputs	The plugin subclass should set it to the minimum number of inputs
	///						the host must provide.
	void SetMinInputs( unsigned int iMinInputs );
	/// This method is called by a plugin subclass, derived from this class, to indicate the maximum number
	/// of inputs the plugin can receive. This method is usually called when a plugin object is instantiated
	/// (i.e., in the plugin subclass constructor).
	///
	/// \param	iMaxInputs	The plugin subclass should set it to the maximum number of inputs the plugin
	///						can receive.
	void SetMaxInputs( unsigned int iMaxInputs );

	/// This method is called by a plugin subclass, derived from this class, to indicate whether the
	/// SetTime function is supported
	///
	/// \param	supported	The plugin indicates whether it supports the SetTime function by passing true or false (1 or 0)
	void SetTimeSupported( bool supported );

	/// This method is called by a plugin subclass, derived from this class, to specify name, type, and default
	/// value of the plugin parameter whose index is passed as parameter to the method. This method is usually
	/// called when a plugin object is instantiated (i.e., in the plugin subclass contructor). This version of
	/// the SetParamInfo function (DefaultValue of type float) should be called for all types of plugin parameters
	/// except for text, boolean, and event parameters.
	///
	/// \param	dwIndex			Index of the plugin parameter whose data are specified.
	///							It should be in the range [0, Number of plugin parameters).
	/// \param	pchName			A string containing the name of the plugin parameter.
	///							According to the FreeFrame specification it should be at most 16 1-byte ASCII
	///							characters long. Longer strings will be truncated at the 16th character.
	/// \param	dwType			The type of the plugin parameter. Codes for allowed types are defined in FreeFrame.h.
	/// \param	fDefaultValue	The default value of the plugin parameter. According to the FreeFrame
	///							specification it must be a float in the range [0, 1].
	void SetParamInfo( unsigned int index, const char* pchName, unsigned int type, float fDefaultValue );
	/// This method is called by a plugin subclass, derived from this class, to specify name, type, and default
	/// value of the plugin parameter whose index is passed as parameter to the method. This method is usually
	/// called when a plugin object is instantiated (i.e., in the plugin subclass contructor). This version of
	/// the SetParamInfo function (DefaultValue of type bool) should be called for plugin parameters of type
	/// boolean or event.
	///
	/// \param	dwIndex			Index of the plugin parameter whose data are specified.
	///							It should be in the range [0, Number of plugin parameters).
	/// \param	pchName			A string containing the name of the plugin parameter.
	///							According to the FreeFrame specification it should be at most 16 1-byte ASCII
	///							characters long. Longer strings will be truncated at the 16th character.
	/// \param	dwType			The type of the plugin parameter. Codes for allowed types are defined in FreeFrame.h.
	/// \param	bDefaultValue	The boolean default value of the plugin parameter.
	void SetParamInfo( unsigned int index, const char* pchName, unsigned int type, bool bDefaultValue );
	/// This method is called by a plugin subclass, derived from this class, to specify name, type, and default
	/// value of the plugin parameter whose index is passed as parameter to the method. This method is usually
	/// called when a plugin object is instantiated (i.e., in the plugin subclass contructor). This version of
	/// the SetParamInfo function (DefaultValue of type char*) should be called for plugin parameters of type text.
	///
	/// \param	dwIndex			Index of the plugin parameter whose data are specified.
	///							It should be in the range [0, Number of plugin parameters).
	/// \param	pchName			A string containing the name of the plugin parameter.
	///							According to the FreeFrame specification it should be at most 16 1-byte ASCII
	///							characters long. Longer strings will be truncated at the 16th character.
	/// \param	dwType			The type of the plugin parameter. Codes for allowed types are defined in FreeFrame.h.
	/// \param	pchDefaultValue	A string to be used as the default value of the plugin parameter.
	void SetParamInfo( unsigned int index, const char* pchName, unsigned int type, const char* pchDefaultValue );

	/// This method is called by a plugin subclass, derived from this class, to specify name, type, and default
	/// value of the plugin parameter whose index is passed as parameter to the method. This method is usually
	/// called when a plugin object is instantiated (i.e., in the plugin subclass contructor). This version of
	/// the SetParamInfo function (DefaultValue of type float) should be called for all types of plugin parameters
	/// except for text, boolean, and event parameters.
	///
	/// \param	dwIndex			Index of the plugin parameter whose data are specified.
	///							It should be in the range [0, Number of plugin parameters).
	/// \param	pchName			A string containing the name of the plugin parameter.
	///							According to the FreeFrame specification it should be at most 16 1-byte ASCII
	///							characters long. Longer strings will be truncated at the 16th character.
	/// \param	numElements		Number of elements of this parameter ( array )
	/// \param	usage			Usage of this parameter,
	void SetBufferParamInfo( unsigned int dwIndex, const char* pchName, unsigned int numElements, unsigned int usage );
	/// This method is called by a plugin subclass, derived from this class, to specify name, type, and default
	/// value of the plugin parameter whose index is passed as parameter to the method. This method is usually
	/// called when a plugin object is instantiated (i.e., in the plugin subclass contructor). This version of
	/// the SetParamInfo function (DefaultValue of type float) should be called for all types of plugin parameters
	/// except for text, boolean, and event parameters.
	///
	/// \param	dwIndex			Index of the plugin parameter whose data are specified.
	///							It should be in the range [0, Number of plugin parameters).
	/// \param	pchName			A string containing the name of the plugin parameter.
	///							According to the FreeFrame specification it should be at most 16 1-byte ASCII
	///							characters long. Longer strings will be truncated at the 16th character.
	/// \param	numElements		Number of elements of this parameter ( array )
	void SetOptionParamInfo( unsigned int dwIndex, const char* pchName, unsigned int numElements, float defaultValue );
	void SetParamElementInfo( unsigned int paramID, unsigned int elementIndex, const char* elementName, float elementValue );
	/// Request the host to add a separator in front of an option parameter's element.
	/// Most hosts will show option parameters as a drop down, when doing this it's sometimes usefull to
	/// have categories in the dropdown. These separators can be used to create those categories.
	///
	/// \param	paramID				The index of the option parameter which contains the element for which
	///								the value is to be changed.
	///								It should be the id of an option parameter.
	///	\param	beforeElementIndex	The index of the element before which the separator should be shown.
	///								It should be in the range [0, parameter.Number of elements).
	void AddElementSeparator( unsigned int paramID, unsigned int beforeElementIndex );

	void SetFileParamInfo( unsigned int index, const char* pchName, std::vector< std::string > supportedExtensions );

	/// Sets whether or not a parameter should be visible in the host's ui.
	///
	/// \param paramID			Index of the parameter whose visibility has to be changed.
	/// \param shouldBeVisible	True if the parameter should be visible in the ui, false otherwise.
	void SetParamVisibility( unsigned int paramID, bool shouldBeVisible );
	void SetParamRange( unsigned int index, float min, float max );

	/// Raises an event flag on a certain parameter. Calling this will store the event as being a pending event
	/// untill the host decides to consume the event and handles it. Raising an event multiple times before the host
	/// consumes them will only output a single event to the host. This is okay because the expected host's response
	/// is to query the parameter's state again, so if it has updated twice then handling the event once after both updates
	/// will still result in the host querying the correct state.
	///
	/// \param paramID			Index of the parameter for which you want the event to be raised.
	/// \param eventToRaise		The event flag to raise for the parameter. This has to be one of the FF_EVENT_FLAG_ flags.
	void RaiseParamEvent( unsigned int paramID, FFUInt64 eventToRaise );

protected:
	// Structure for keeping information about each plugin parameter
	struct ParamInfo
	{
		ParamInfo() :
			range{ 0, 1 }
		{
		}

		unsigned int ID;
		char Name[ 16 ];
		unsigned int dwType;

		// extra parameters
		struct Element
		{
			std::string name;
			float value = 0.0f;
		};
		std::vector< Element > elements;
		struct ElementSeparator
		{
			unsigned int beforeIndex;
		};
		std::vector< ElementSeparator > elementSeparators;
		unsigned int usage;

		bool visibleInUI = true;
		RangeStruct range;

		float defaultFloatVal = 0.0f;
		std::string defaultStringVal;
		std::vector< std::string > supportedExtensions; //!< The extensions this parameter supports. Only used if dwType is FF_TYPE_FILE.

		FFUInt64 pendingEventFlags = 0;                 //!< Event flags for events that are pending for the current parameter.
	};
	enum class TextureOrientation
	{
		BOTTOM_LEFT,
		TOP_LEFT
	};

	ParamInfo* FindParamInfo( unsigned int ID );
	const ParamInfo* FindParamInfo( unsigned int ID ) const;
	/// Get the current texture orientation. Plugin subclasses that support the top-left texture orientation will call this
	/// to know if the top-left orientation has been enabled by the host.
	TextureOrientation GetTextureOrientation() const;

private:
	std::vector< ParamInfo > params;

	// Inputs
	int m_iMinInputs;
	int m_iMaxInputs;

	bool m_timeSupported;                            //!< Whether or not this plugin supports having it's time set.
	const bool m_topLeftTextureOrientationSupported; //!< Whether or not this plugin supports input/output textures with the top-left orientation rather than OpenGL's standard bottom-right.
	TextureOrientation textureOrientation;  //!< The texture orientation the host/plugin have agreed to use. By default plugins use OpenGL's bottom_left standard.
};

#endif
