#include "SmoothnessTester.h"

#include "bar.h.glsl"

using namespace ffglex;

#define PARAM_Width ( 0 )
#define PARAM_Compare ( 1 )

static CFFGLPluginInfo PluginInfo(
	PluginFactory< SmoothnessTester >,      // Create method
	"SS02",                        // Plugin unique ID
	"Smoothness",                  // Plugin name
	2,                             // API major version number
	1,                             // API minor version number
	1,                             // Plugin major version number
	000,                           // Plugin minor version number
	FF_SOURCE,                     // Plugin type
	"Smoothness Tester",           // Plugin description
	"Used to visually test the rendering smoothness. The measured frame rate is an average. This plugin allows you to visually check for hiccups."     // About
);

SmoothnessTester::SmoothnessTester() :
	m_Width( 0.05f )
{
	// Input properties
	SetMinInputs( 0 );
	SetMaxInputs( 0 );

	// Parameters
	SetParamInfof( PARAM_Width, "Width", FF_TYPE_STANDARD );
	SetParamInfof( PARAM_Compare, "Compare", FF_TYPE_BOOLEAN );
}
FFResult SmoothnessTester::InitGL( const FFGLViewportStruct* vp )
{
	bool success = barShader.Compile( bar::vertex, bar::fragment );
	success &= quad.Initialise();

	if( !success )
	{
		DeInitGL();
		return FF_FAIL;
	}

	//Use base-class init as success result so that it retains the viewport.
	return CFFGLPlugin::InitGL( vp );
}
FFResult SmoothnessTester::ProcessOpenGL( ProcessOpenGLStruct* pGL )
{
	ScopedShaderBinding shaderBinding( barShader.GetGLID() );

	barShader.Set( "Width", m_Width );
	if ( m_Compare ) {
		barShader.Set( "Time", GetElapsedSeconds() );
	} else {
		barShader.Set( "Time", (float) hostTime / 1000.f );
	}

	quad.Draw();

	return FF_SUCCESS;
}

FFResult SmoothnessTester::DeInitGL()
{
	barShader.FreeGLResources();
	quad.Release();

	return FF_SUCCESS;
}

FFResult SmoothnessTester::SetFloatParameter( unsigned int dwIndex, float value )
{
	switch( dwIndex )
	{
	case PARAM_Width:
		m_Width = value;
		break;
	case PARAM_Compare:
		m_Compare = value > 0.5;
		break;
	default:
		return FF_FAIL;
	}

	return FF_SUCCESS;
}

float SmoothnessTester::GetFloatParameter( unsigned int index )
{
	switch( index )
	{
	case PARAM_Width:
		return m_Width;
		break;
	case PARAM_Compare:
		return m_Compare ? 1.0f : 0.0f;
		break;
	default:
		break;
	}

	return 0.0f;
}
