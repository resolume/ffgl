#pragma once
#include <vector>
#include <string>
#include <chrono>
#include "../ffgl/FFGLPluginInfo.h"
#include "../ffgl/FFGLPluginSDK.h"
#include "../ffgl/FFGLLib.h"
#include "../ffglex/FFGLShader.h"
#include "../ffglex/FFGLScreenQuad.h"
#include "../ffglex/FFGLUtilities.h"
#include "../ffglex/FFGLScopedShaderBinding.h"
#include "FFGLUtils.h"
#include "FFGLAudio.h"
#include "FFGLRandom.h"

namespace ffglqs
{
class Param;
class ParamRange;
class ParamOption;
class ParamFFT;
class ParamText;

typedef CFFGLPluginInfo PluginInstance;

struct PluginInfo
{
	std::string id, name, description, about;
	int majorVersion = 1;
	int minorVersion = 0;
	PluginInfo( std::string id, std::string name ) :
		id( id ), name( name )
	{
	}
};

class Plugin : public CFFGLPlugin
{
public:
	Plugin( bool supportTopLeftTextureOrientation = false );
	~Plugin();

	/// This method allows to create the plugin description.
	/// This is what makes your plugin available in the exported library file.
	template< typename PluginType >
	static PluginInstance CreatePlugin( PluginInfo infos, FFUInt32 type );

	/// Implementation of the FFGL InitGL instance specific function. This function allocates
	/// the OpenGL resources the plugin needs during its lifetime. If the shader fails to
	/// compile we return FF_FAIL.
	///
	/// \param	viewPort		Pointer to a FFGLViewportStruct structure (see the definition in FFGL.h
	///							and the description in the FFGL specification).
	/// \return					This implementation returns FF_SUCCESS if the shader did compile
	///							correctly FF_FAIL otherwise.
	FFResult InitGL( const FFGLViewportStruct* viewPort ) override;
	/// Implementation of the FFGL ProcessOpenGL instance specific function. It is in this function that
	/// the actual rendering occur.
	///
	/// \param	inputTextures	This is a parameter containing info about input texture if there is some.
	/// \return					This implementation always returns FF_SUCCESS.
	FFResult ProcessOpenGL( ProcessOpenGLStruct* inputTextures ) override;
	/// Implementation of the FFGL DeInitGL instance specific function. This function frees
	/// any OpenGL resources the plugin has allocated
	///
	/// \return					This implementation always returns FF_SUCCESS.
	FFResult DeInitGL() override;

	/// You can override this function to allocate any ressource you will need, shader for example. If any error
	/// occured (shader fails to compile for example), return FF_FAIL.
	/// \return                 FF_FAIL if any error occured else FF_SUCCESS
	virtual FFResult Init()
	{
		return FF_SUCCESS;
	}
	/// You can override this function to do something just before the main shader call. It can for example be usefull
	/// if you have to react to certains parameter change in a specific way and / or send custom uniform to the shader
	virtual void Update()
	{
	}
	/// The default implementation will just call glDraw with the main shader. You can override this if your plugin needs
	/// to do more complex things, like calling multiple shaders.
	/// \return                 FF_FAIL if any error occured else FF_SUCCESS
	virtual FFResult Render( ProcessOpenGLStruct* inputTextures );
	/// Use this function to free any ressource allocated in your init() function
	virtual void Clean()
	{
	}

	/// This function allows to include snippet of code that are provided in Utils.h. This allows plugin
	/// programmer to not rewrite common code in each plugin. For example, code to get a random number,
	/// map a value from a range to another, and simplex noise are currently available.
	/// \param	snippet		The id of a snippet to include.
	void Include( shader::snippet_id snippet );
	/// This function will call the function just above for each snippets in the set.
	/// \param	snippets		A set of snippets to include.
	void Include( std::set< shader::snippet_id > snippets );
	/// In this function we assemble the different bit of the fragment shader into one and try to compile it.
	/// This allows to add all the default uniform to the begining of the shader so you don't
	/// have to do it manually. It also permits to add any included snippet to it.
	/// \param	base		The base of your shader, usually just the void main() function
	/// \return				The full shader as a string
	std::string CreateFragmentShader( std::string base );
	/// This function is called by the plugin before each render it allows to update all the params that
	/// are sent to the main shader.
	void UpdateAudioAndTime();
	/// This function will send all the parameter registered by your plugin to the shader.
	/// \param	shader		The shader to send the params
	void SendParams( ffglex::FFGLShader& shader );
	/// This function will send all the default parameter (like time, bpm, resolution...) to the shader.
	/// \param	shader		The shader to send the params
	void SendDefaultParams( ffglex::FFGLShader& shader );

	/// This function is called by the host to get a string representation of any parameter.
	/// It will just return the value of the current param as a char*.
	/// \param	index		The index of the parameter to display
	/// \return				The char* representation of the parameter
	char* GetParameterDisplay( unsigned int index ) override;
	/// This function is called by the host to set a value to a specific param thanks to its index.
	/// \param	index		The index of the parameter to set
	/// \param	value		The value to assign to the param
	/// \return				FF_SUCCESS if we managed to find the parameter and set its value
	FFResult SetFloatParameter( unsigned int index, float value ) override;
	/// This function is called by the host to get the float value of a Param thanks to its index.
	/// \param	index		The index of the parameter to get
	/// \return				The float value of the parameter
	float GetFloatParameter( unsigned int index ) override;
	/// This function is called by the host to set the text value of a ParamText.
	/// \param	index		The index of the parameter to set
	/// \param	value		The text to assign to the param
	/// \return				FF_SUCCESS if we managed to find the parameter and set its value
	FFResult SetTextParameter( unsigned int index, const char* value ) override;
	/// This function is called by the host to get the text value of a ParamText.
	/// \param	index		The index of the parameter to get
	/// \return				The text assigned to the parameter
	char* GetTextParameter( unsigned int index ) override;
	/// This function is called by the host when the current sample rate has changed.
	/// \param	sampleRate	The new sample rate
	void SetSampleRate( unsigned int sampleRate ) override;

	/// This function must be called in the constructor of your plugin. It will allows to
	/// construct the full fragment shader. In order to simplify the development of plugin
	/// it is InitGL that takes care of defining all the parameters so that the shader can acces
	/// them.
	/// \param	base		The fragment shader
	void SetFragmentShader( std::string base );
	/// This function allows you to add a new parameter to the plugin. There is differents kind of
	/// parameters available, you can check them in Params.h. Adding a parameter allows the plugin to be
	/// aware of them, make them available to the host and take of all the communication with it.
	/// It also allows the plugin to automatically pass the current value of each parameters to the
	/// shader before drawing.
	/// \param	param		The parameter to add
	void AddParam( std::shared_ptr< Param > param );
	/// This function handle the special case where the parameter is a ParamRange
	/// \param	param		The parameter to add
	void AddParam( std::shared_ptr< ParamRange > param );
	/// This function handle the special case where the parameter is a ParamOption (When you have the
	/// choice between different option).
	/// \param	param		The parameter to add
	void AddParam( std::shared_ptr< ParamOption > param );
	/// This function handle the special case where the parameter is a ParamFFT
	/// \param	param		The parameter to add
	void AddParam( std::shared_ptr< ParamFFT > fft );
	/// This function allows to create a Hue color param, for exemple in Resolume this will display a
	/// color picker, which is very handy to choose your color.
	/// \param	name		The name of the parameter to add
	void AddHueColorParam( std::string name );
	/// This function allows to create a RGB color param, in Resolume it is displayed as three sliders for
	/// each Red, Green and Blue.
	/// \param	name		The name of the parameter to add
	void AddRGBColorParam( std::string name );
	/// This function allows to check if a certains index corresponds to a Hue color. Internally, it is
	/// represented as three Params. It checks if the current one and the two followings are actually
	/// corresponding to a Hue color.
	/// \param	index		The index of the first parameter that could correspond to a Hue color.
	bool IsHueColor( int index );
	/// This function allows to check if a certains index corresponds to a RGB color. Internally, it is
	/// represented as three Params. It checks if the current one and the two followings are actually
	/// corresponding to a RGB color.
	/// \param	index		The index of the first parameter that could correspond to a RGB color.
	bool IsRGBColor( int index );

	/// This function allows to get a parameter by it's name.
	/// \param	name		The name of the parameter to get.
	/// \return				The parameter if we managed to find it.
	std::shared_ptr< Param > GetParam( std::string name );
	/// This function allows to get an option parameter by it's name.
	/// \param	name		The name of the parameter to get.
	/// \return				The parameter if we managed to find it.
	std::shared_ptr< ParamOption > GetParamOption( std::string name );
	/// This function allows to get a text parameter by it's name.
	/// \param	name		The name of the parameter to get.
	/// \return				The parameter if we managed to find it.
	std::shared_ptr< ParamText > GetParamText( std::string name );

protected:
	std::string fragmentShaderBase;
	std::vector< std::shared_ptr< Param > > params;
	std::map< std::shared_ptr< ParamFFT >, Audio > audioParams;
	ffglex::FFGLShader shader;
	ffglex::FFGLScreenQuad quad;

	float timeNow                                                         = 0;
	float deltaTime                                                       = 0;
	float lastUpdate                                                      = 0;
	int frame                                                             = 0;
	std::chrono::time_point< std::chrono::high_resolution_clock > t_start = std::chrono::high_resolution_clock::now();
	Random random;
	std::set< shader::snippet_id > includedSnippets;

	std::string fragmentShaderCodeStart = R"(
		#version 410 core
		out vec4 fragColor;
		uniform vec2 resolution;
		uniform float time;
		uniform float deltaTime;
		uniform int frame;
		uniform float bpm;
		uniform float phase;
	)";
	std::string vertexShaderCode;

	// This function will consume the value for all ParamTrigger
	void consumeAllTrigger();
};

template< typename PluginType >
inline PluginInstance Plugin::CreatePlugin( PluginInfo infos, FFUInt32 type )
{
	return PluginInstance(
		PluginFactory< PluginType >,// Create method, allows the host to create a new instance of the plugin
		infos.id.c_str(),           // Plugin unique ID
		infos.name.c_str(),         // Plugin name
		2,                          // API major version number
		1,                          // API minor version number
		infos.majorVersion,         // Plugin major version number
		infos.minorVersion,         // Plugin minor version number
		type,                       // Plugin type
		infos.description.c_str(),  // Plugin description
		infos.about.c_str()         // About
	);
}

}//End namespace ffglqs
