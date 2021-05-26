#include "FFGLEvents.h"
#include <math.h>//floor
using namespace ffglex;

enum ParamType : FFUInt32
{
	PT_RANDOMIZE,
	PT_FLOAT,
	PT_OPTION,
	PT_TEXT,
	PT_RED,
	PT_GREEN,
	PT_BLUE,
	PT_ALPHA,
	PT_HUE,
	PT_SAT,
	PT_BRI,
	PT_ALPHA2
};

static CFFGLPluginInfo PluginInfo(
	PluginFactory< FFGLEvents >,//Create method
	"RS04",                     //Plugin unique ID
	"Events Example",           //Plugin name
	2,                          //API major version number
	1,                          //API minor version number
	1,                          //Plugin major version number
	0,                          //Plugin minor version number
	FF_SOURCE,                  //Plugin type
	"Sample FFGL Events plugin",//Plugin description
	"Resolume FFGL Example"     //About
);

FFGLEvents::FFGLEvents() :
	floatParam( 0.5f ),
	optionParam( 0.0f ),
	textParam( "Unchanged Text" )
{
	rgba[ 0 ] = rgba[ 1 ] = rgba[ 2 ] = rgba[ 3 ] = 1.0f;
	hsba[ 0 ] = hsba[ 1 ] = hsba[ 2 ] = hsba[ 3 ] = 1.0f;

	//Input properties
	SetMinInputs( 0 );
	SetMaxInputs( 0 );

	//Parameters
	SetParamInfof( PT_RANDOMIZE, "Randomize", FF_TYPE_EVENT );//The button that'll cause parameter values/names etc to change.
	SetParamInfof( PT_FLOAT, "Float", FF_TYPE_STANDARD );
	SetOptionParamInfo( PT_OPTION, "Option", 3, optionParam );
	SetParamElementInfo( PT_OPTION, 0, "Unchanged 0", 0.0f );
	SetParamElementInfo( PT_OPTION, 1, "Unchanged 1", 1.0f );
	SetParamElementInfo( PT_OPTION, 2, "Unchanged 2", 2.0f );
	SetParamInfo( PT_TEXT, "Text", FF_TYPE_TEXT, textParam.c_str() );
	SetParamInfof( PT_RED, "Red", FF_TYPE_RED );
	SetParamInfof( PT_GREEN, "Green", FF_TYPE_GREEN );
	SetParamInfof( PT_BLUE, "Blue", FF_TYPE_BLUE );
	SetParamInfof( PT_ALPHA, "Alpha", FF_TYPE_ALPHA );
	SetParamInfof( PT_HUE, "Color", FF_TYPE_HUE );
	SetParamInfof( PT_SAT, "Color_Sat", FF_TYPE_SATURATION );
	SetParamInfof( PT_BRI, "Color_Bri", FF_TYPE_BRIGHTNESS );
	SetParamInfof( PT_ALPHA2, "Color_Alpha", FF_TYPE_ALPHA );

	FFGLLog::LogToHost( "Created FFGLEvents generator" );
}
FFResult FFGLEvents::ProcessOpenGL( ProcessOpenGLStruct* pGL )
{
	//This example isn't about rendering things. We're just showing how a plugin can change it's parameters' properties
	//and make the host pick that up.
	return FF_SUCCESS;
}

FFResult FFGLEvents::SetFloatParameter( unsigned int dwIndex, float value )
{
	switch( dwIndex )
	{
	case PT_RANDOMIZE:
		if( value != 0.0f )
			Randomize();//On button press we randomize our params so you can see how to change something and make the host pick it up.
		break;
	case PT_FLOAT:
		floatParam = value;
		break;
	case PT_OPTION:
		optionParam = value;
		break;
	case PT_RED:
		rgba[ 0 ] = value;
		break;
	case PT_GREEN:
		rgba[ 1 ] = value;
		break;
	case PT_BLUE:
		rgba[ 2 ] = value;
		break;
	case PT_ALPHA:
		rgba[ 3 ] = value;
		break;
	case PT_HUE:
		hsba[ 0 ] = value;
		break;
	case PT_SAT:
		hsba[ 1 ] = value;
		break;
	case PT_BRI:
		hsba[ 2 ] = value;
		break;
	case PT_ALPHA2:
		hsba[ 3 ] = value;
		break;

	default:
		return FF_FAIL;
	}

	return FF_SUCCESS;
}
FFResult FFGLEvents::SetTextParameter( unsigned int index, const char* value )
{
	switch( index )
	{
	case PT_TEXT:
		textParam = value;
		break;

	default:
		return FF_FAIL;
	}

	return FF_SUCCESS;
}

float FFGLEvents::GetFloatParameter( unsigned int index )
{
	switch( index )
	{
	case PT_FLOAT:
		return floatParam;
	case PT_OPTION:
		return optionParam;
	case PT_RED:
		return rgba[ 0 ];
	case PT_GREEN:
		return rgba[ 1 ];
	case PT_BLUE:
		return rgba[ 2 ];
	case PT_ALPHA:
		return rgba[ 3 ];
	case PT_HUE:
		return hsba[ 0 ];
	case PT_SAT:
		return hsba[ 1 ];
	case PT_BRI:
		return hsba[ 2 ];
	case PT_ALPHA2:
		return hsba[ 3 ];
	}

	return 0.0f;
}
char* FFGLEvents::GetTextParameter( unsigned int index )
{
	static char textBufferForHost[ 512 ];
	switch( index )
	{
	case PT_TEXT:
		strcpy( textBufferForHost, textParam.c_str() );
		break;
	}

	return textBufferForHost;
}

void FFGLEvents::Randomize()
{
	//Visibility changes. The true argument needs to be passed when changing visibility on the fly, it can
	//be false when calling this function during the constructor for initial setup.
	//SetParamVisibility( PT_FLOAT, rand() % 2 == 1, true );
	//SetParamVisibility( PT_OPTION, rand() % 2 == 1, true );
	//SetParamVisibility( PT_TEXT, rand() % 2 == 1, true );
	////RGBA parameters work just like standard parameters, they are controlled by their own param id...
	//SetParamVisibility( PT_GREEN, rand() % 2 == 1, true );//Note how this doesn't affed red/blue/alpha
	////... HSBA parameters however, they're entirely controlled through the handle of the hue parameter.
	////So this call will toggle visibility for all four parameters (hue, saturation, brightness, alpha2)
	//SetParamVisibility( PT_HUE, rand() % 2 == 0, true );//Note how this DOES affect saturation/brightness/alpha2

	//Display name changes. The true argument needs to be passed when changing visibility on the fly, it can
	//be false when calling this function during the constructor for initial setup.
	SetParamDisplayName( PT_FLOAT, std::string( "Float " ) + std::to_string( rand() % 1000 ), true );
	SetParamDisplayName( PT_OPTION, std::string( "Option " ) + std::to_string( rand() % 1000 ), true );
	SetParamDisplayName( PT_TEXT, std::string( "Text " ) + std::to_string( rand() % 1000 ), true );
	SetParamDisplayName( PT_GREEN, std::string( "Green " ) + std::to_string( rand() % 1000 ), true );//Doesn't affect red/blue/alpha
	//Similarly to visibility changes, the hue parameter is used to control the hsba color parameter's display name.
	SetParamDisplayName( PT_HUE, std::string( "HSBA " ) + std::to_string( rand() % 1000 ), true );//Does affect saturation/brightness/alpha2

	//The param's value must remain within it's range. By default parameters should be between 0.0f and 1.0f
	floatParam = rand() / (float)RAND_MAX;
	RaiseParamEvent( PT_FLOAT, FF_EVENT_FLAG_VALUE );

	//Make sure that the param value exactly matches a value of one of it's elements otherwise the
	//host wouldn't know which element is currently active.
	optionParam = static_cast< float >( rand() % 30 );
	RaiseParamEvent( PT_OPTION, FF_EVENT_FLAG_VALUE );

	textParam = std::string( "Changed Text " ) + std::to_string( rand() % 1000 );
	RaiseParamEvent( PT_TEXT, FF_EVENT_FLAG_VALUE );

	//In rgba mode we can change just a single channel.
	rgba[ 1 ] = rand() / (float)RAND_MAX;
	RaiseParamEvent( PT_GREEN, FF_EVENT_FLAG_VALUE );

	//Even though we're only changing the saturation value, we still need to raise the event for the hue
	//parameter's id because that's the id that the host will use for a hsba based colorpicker.
	hsba[ 1 ] = rand() / (float)RAND_MAX;
	RaiseParamEvent( PT_HUE, FF_EVENT_FLAG_VALUE );
}