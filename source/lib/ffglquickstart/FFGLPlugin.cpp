#include "FFGLPlugin.h"
#include "FFGLParamRange.h"
#include "FFGLParamOption.h"
#include "FFGLParamFFT.h"
#include "FFGLParamText.h"
#include "FFGLParamTrigger.h"
using namespace ffglex;

namespace ffglqs
{
Plugin::Plugin( bool supportTopLeftTextureOrientation ) :
	CFFGLPlugin( supportTopLeftTextureOrientation )
{
}

Plugin::~Plugin()
{
}

FFResult Plugin::InitGL( const FFGLViewportStruct* viewPort )
{
	std::string fragmentShaderCode = CreateFragmentShader( fragmentShaderBase );
	if( !shader.Compile( vertexShaderCode, fragmentShaderCode ) )
	{
		DeInitGL();
		return FF_FAIL;
	}
	if( !quad.Initialise() )
	{
		DeInitGL();
		return FF_FAIL;
	}
	if( Init() == FF_FAIL )
	{
		DeInitGL();
		return FF_FAIL;
	}

	return CFFGLPlugin::InitGL( viewPort );
}

FFResult Plugin::ProcessOpenGL( ProcessOpenGLStruct* inputTextures )
{
	UpdateAudioAndTime();
	//Activate our shader using the scoped binding so that we'll restore the context state when we're done.
	ScopedShaderBinding shaderBinding( shader.GetGLID() );
	SendDefaultParams( shader );
	SendParams( shader );
	Update();
	FFResult result = Render( inputTextures );
	consumeAllTrigger();

	return result;
}

FFResult Plugin::DeInitGL()
{
	shader.FreeGLResources();
	quad.Release();
	Clean();
	return FF_SUCCESS;
}

FFResult Plugin::Render( ProcessOpenGLStruct* inputTextures )
{
	//Activate our shader using the scoped binding so that we'll restore the context state when we're done.
	ScopedShaderBinding shaderBinding( shader.GetGLID() );
	quad.Draw();
	return FF_SUCCESS;
}

std::string Plugin::CreateFragmentShader( std::string base )
{
	std::string fragmentShaderCode = fragmentShaderCodeStart;
	int i                          = 0;
	while( i < params.size() )
	{
		if( IsRGBColor( i ) )
		{
			fragmentShaderCode += "uniform vec3 " + params[ i ]->GetName() + ";\n";
			i += 2;
		}
		else if( IsHueColor( i ) )
		{
			fragmentShaderCode += "uniform vec4 " + params[ i ]->GetName() + ";\n";
			i += 3;
		}
		else if( params[ i ]->GetType() == FF_TYPE_BOOLEAN || params[ i ]->GetType() == FF_TYPE_EVENT )
		{
			fragmentShaderCode += "uniform bool " + params[ i ]->GetName() + ";\n";
		}
		else if( params[ i ]->GetType() != FF_TYPE_BUFFER )
		{
			fragmentShaderCode += "uniform float " + params[ i ]->GetName() + ";\n";
		}
		i += 1;
	}

	for( auto snippet : includedSnippets )
	{
		fragmentShaderCode += shader::snippets.find( snippet )->second;
	}

	fragmentShaderCode += base;
	return fragmentShaderCode;
}

void Plugin::UpdateAudioAndTime()
{
	// Update time and frame data
	frame++;
	auto t_now = std::chrono::high_resolution_clock::now();
	timeNow    = std::chrono::duration< float, std::milli >( t_now - t_start ).count() / 1000.0f;
	deltaTime  = timeNow - lastUpdate;
	lastUpdate = timeNow;

	// Update FFT data
	for( auto entry : audioParams )
	{
		std::shared_ptr< ParamFFT > param = entry.first;
		ParamInfo* fftInfo                = FindParamInfo( param->index );
		for( size_t index = 0; index < param->fftData.size(); ++index )
			param->fftData[ index ] = fftInfo->elements[ index ].value;
		audioParams[ param ].Update( param->fftData );
	}
}

void Plugin::SendParams( FFGLShader& shader )
{
	int i = 0;
	while( i < params.size() )
	{
		if( IsRGBColor( i ) )
		{
			std::string name = params[ i ]->GetName();
			float r          = params[ i ]->GetValue();
			float g          = params[ i + 1 ]->GetValue();
			float b          = params[ i + 2 ]->GetValue();
			shader.Set( name.c_str(), r, g, b );
			i += 2;
		}
		else if( IsHueColor( i ) )
		{
			float rgb[ 3 ];
			std::string name = params[ i ]->GetName();
			float hue        = params[ i ]->GetValue();
			float saturation = params[ i + 1 ]->GetValue();
			float brightness = params[ i + 2 ]->GetValue();
			float alpha      = params[ i + 3 ]->GetValue();
			//we need to make sure the hue doesn't reach 1.0f, otherwise the result will be pink and not red how it should be
			hue = ( hue == 1.0f ) ? 0.0f : hue;
			HSVtoRGB( hue, saturation, brightness, rgb[ 0 ], rgb[ 1 ], rgb[ 2 ] );
			shader.Set( name.c_str(), rgb[ 0 ], rgb[ 1 ], rgb[ 2 ], alpha );
			i += 3;
		}
		if( params[ i ]->GetType() == FF_TYPE_BOOLEAN || params[ i ]->GetType() == FF_TYPE_EVENT )
		{
			shader.Set( params[ i ]->GetName().c_str(), (bool)params[ i ]->GetValue() );
		}
		else if( params[ i ]->GetType() != FF_TYPE_BUFFER )
		{
			shader.Set( params[ i ]->GetName().c_str(), params[ i ]->GetValue() );
		}
		i += 1;
	}
}

void Plugin::SendDefaultParams( ffglex::FFGLShader& shader )
{
	shader.Set( "time", timeNow );
	shader.Set( "deltaTime", deltaTime );
	shader.Set( "frame", frame );
	shader.Set( "resolution", (float)currentViewport.width, (float)currentViewport.height );
	shader.Set( "bpm", bpm );
	shader.Set( "phase", barPhase );
}

char* Plugin::GetParameterDisplay( unsigned int index )
{
	bool inRange = 0 <= index && index < params.size();
	bool valid   = params[ index ]->GetType() != FF_TYPE_TEXT && params[ index ]->GetType() != FF_TYPE_FILE;
	if( inRange && valid )
	{
		static char displayValueBuffer[ 16 ];
		float value             = params[ index ]->GetValue();
		std::string stringValue = std::to_string( value );
		memset( displayValueBuffer, 0, sizeof( displayValueBuffer ) );
		memcpy( displayValueBuffer, stringValue.c_str(), std::min( sizeof( displayValueBuffer ), stringValue.length() ) );
		return displayValueBuffer;
	}
	else
	{
		return (char*)FF_FAIL;
	}
}

FFResult Plugin::SetFloatParameter( unsigned int index, float value )
{
	if( index < params.size() )
	{
		params[ index ]->SetValue( value );
		return FF_SUCCESS;
	}
	else
	{
		return FF_FAIL;
	}
}

float Plugin::GetFloatParameter( unsigned int index )
{
	if( 0 <= index && index < params.size() )
	{
		return params[ index ]->GetValue();
	}
	else
	{
		return 0.0f;
	}
}

FFResult Plugin::SetTextParameter( unsigned int index, const char* value )
{
	bool inBounds = 0 <= index && index < params.size();
	if( !inBounds )
		return FF_FAIL;

	auto paramText = std::dynamic_pointer_cast< ParamText >( params[ index ] );
	if( !paramText )
		return FF_FAIL;

	paramText->text = value == nullptr ? "" : value;
	return FF_SUCCESS;
}

char* Plugin::GetTextParameter( unsigned int index )
{
	bool inBounds = 0 <= index && index < params.size();
	if( !inBounds )
		return (char*)FF_FAIL;

	auto paramText = std::dynamic_pointer_cast< ParamText >( params[ index ] );
	if( !paramText )
		return (char*)FF_FAIL;

	return const_cast< char* >( paramText->text.c_str() );
}

void Plugin::SetSampleRate( unsigned int _sampleRate )
{
	sampleRate = _sampleRate;
	for( auto entry : audioParams )
	{
		std::shared_ptr< ParamFFT > param = entry.first;
		audioParams[ param ].SetSampleRate( _sampleRate );
	}
}

void Plugin::SetFragmentShader( std::string base )
{
	fragmentShaderBase = base;
}

void Plugin::AddParam( std::shared_ptr< Param > param )
{
	unsigned int new_index = (unsigned int)params.size();
	SetParamInfo( new_index, param->GetName().c_str(), param->GetType(), param->GetValue() );
	params.push_back( param );
}

void Plugin::AddParam( std::shared_ptr< ParamRange > param )
{
	unsigned int new_index = (unsigned int)params.size();
	SetParamInfo( new_index, param->GetName().c_str(), param->GetType(), param->GetValue() );
	//In FFGLPluginManager.cpp, line 274, SetParamInfo clamps the default value to 0...1 in case of FF_TYPE_STANDARD 
	if( param->GetValue() < 0.0f || param->GetValue() > 1.0f )
	{
		ParamInfo* paramInfo = FindParamInfo( new_index );
		if ( paramInfo != nullptr ) 
			paramInfo->defaultFloatVal = param->GetValue();
	}
	SetParamRange( new_index, param->GetRange().min, param->GetRange().max );
	params.push_back( param );
}

void Plugin::AddParam( std::shared_ptr< ParamOption > param )
{
	unsigned int new_index = (unsigned int)params.size();
	SetOptionParamInfo( new_index, param->GetName().c_str(), (unsigned int)param->options.size(), param->GetValue() );

	for( unsigned int i = 0; i < param->options.size(); i++ )
	{
		SetParamElementInfo( new_index, i, param->options[ i ].name.c_str(), (float)i );
	}
	params.push_back( param );
}

void Plugin::AddParam( std::shared_ptr< ParamFFT > param )
{
	audioParams[ param ] = Audio();
	param->index         = (unsigned int)params.size();
	SetBufferParamInfo( param->index, param->GetName().c_str(), static_cast< unsigned int >( param->fftData.size() ), FF_USAGE_FFT );
	params.push_back( param );
}

void Plugin::AddHueColorParam( std::string name )
{
	AddParam( Param::Create( name, FF_TYPE_HUE, 0. ) );
	AddParam( Param::Create( name + "_saturation", FF_TYPE_SATURATION, 0. ) );
	AddParam( Param::Create( name + "_brighthness", FF_TYPE_BRIGHTNESS, 1.0 ) );
	AddParam( Param::Create( name + "_alpha", FF_TYPE_ALPHA, 1.0 ) );
}

void Plugin::AddRGBColorParam( std::string name )
{
	AddParam( Param::Create( name, FF_TYPE_RED, 0.5 ) );
	AddParam( Param::Create( name + "_green", FF_TYPE_GREEN, 0.5 ) );
	AddParam( Param::Create( name + "_blue", FF_TYPE_BLUE, 0.5 ) );
}

bool Plugin::IsHueColor( int index )
{
	bool enoughSpace = index + 3 < params.size();
	if( !enoughSpace )
		return false;
	bool isColorType =
		params[ index ]->GetType() == FF_TYPE_HUE &&
		params[ index + 1 ]->GetType() == FF_TYPE_SATURATION &&
		params[ index + 2 ]->GetType() == FF_TYPE_BRIGHTNESS &&
		params[ index + 3 ]->GetType() == FF_TYPE_ALPHA;

	return isColorType;
}

bool Plugin::IsRGBColor( int index )
{
	bool enoughSpace = index + 2 < params.size();
	if( !enoughSpace )
		return false;
	bool isColorType =
		params[ index ]->GetType() == FF_TYPE_RED &&
		params[ index + 1 ]->GetType() == FF_TYPE_GREEN &&
		params[ index + 2 ]->GetType() == FF_TYPE_BLUE;

	return isColorType;
}

std::shared_ptr< Param > Plugin::GetParam( std::string name )
{
	for( int i = 0; i < params.size(); i++ )
	{
		if( params[ i ]->GetName().compare( name ) == 0 )
			return params[ i ];
	}
	return {};
}

std::shared_ptr< ParamOption > Plugin::GetParamOption( std::string name )
{
	auto param = GetParam( name );
	if( !param )
		return {};
	auto option = std::dynamic_pointer_cast< ParamOption >( param );
	if( !option )
		return {};
	return option;
}

std::shared_ptr< ParamText > Plugin::GetParamText( std::string name )
{
	auto param = GetParam( name );
	if( !param )
		return {};
	auto text = std::dynamic_pointer_cast< ParamText >( param );
	if( !text )
		return {};
	return text;
}

void Plugin::Include( shader::snippet_id snippet )
{
	if( includedSnippets.find( snippet ) != includedSnippets.end() )
		return;

	includedSnippets.insert( snippet );
	auto deps = shader::dependencies.find( snippet );
	if( deps == shader::dependencies.end() )
		return;
	for( auto dep : deps->second )
	{
		includedSnippets.insert( dep );
	}
}

void Plugin::Include( std::set< shader::snippet_id > snippets )
{
	for( auto snippet : snippets )
	{
		Include( snippet );
	}
}

void Plugin::consumeAllTrigger()
{
	for( int i = 0; i < params.size(); i++ )
	{
		auto trigger = std::dynamic_pointer_cast< ParamTrigger >( params[ i ] );
		if( trigger )
		{
			trigger->Consume();
		}
	}
}

}//End namespace ffglqs
