#include "Add.h"
using namespace ffglex;
using namespace ffglqs;

static CFFGLPluginInfo PluginInfo(
	PluginFactory< Add >,                                                                        // Create method
	"RM01",                                                                                      // Plugin unique ID
	"Add Example",                                                                               // Plugin name
	2,                                                                                           // API major version number
	1,                                                                                           // API minor version number
	1,                                                                                           // Plugin major version number
	000,                                                                                         // Plugin minor version number
	FF_MIXER,                                                                                    // Plugin type
	"Blend two videos by adding the first texture to the second texture. Looks like 50Add mixer",// Plugin description
	"Resolume FFGL example"                                                                      // About
);

static const char _vertexShaderCode[] = R"(#version 410 core
uniform vec2 MaxUVDest;
uniform vec2 MaxUVSrc;

layout( location = 0 ) in vec4 vPosition;
layout( location = 1 ) in vec2 vUV;

out vec2 uvDest;
out vec2 uvSrc;

void main()
{
	gl_Position = vPosition;
	uvDest = vUV * MaxUVDest;
	uvSrc = vUV * MaxUVSrc;
}
)";

//This is the shader. It's using to make the transition.
//The code below will be applied by each pixel of your output screen by the Graphic Card
static const char _fragmentShaderCode[] = R"(#version 410 core
uniform sampler2D textureDest;
uniform sampler2D textureSrc;
//the value defined by the slider to switch between the two images
uniform float Opacity;

in vec2 uvDest;
in vec2 uvSrc;

out vec4 fragColor;

void main()
{
	//get the two fragments to mix
	vec4 colorDest = texture( textureDest, uvDest );
	vec4 colorSrc  = texture( textureSrc, uvSrc );

	//here we add the colorSrc r,g,b,a pixel value to the colorDest pixel value according to the mixVal value
	vec4 mix = colorDest + colorSrc * Opacity;

	//Here we use the built-in function min(val1,val2) to get the minimum between val1 and val2 and always keep output pixel value between 0.0 and 1.0
	fragColor = min( mix, 1.0 );
}
)";

Add::Add()
{
	// Input properties
	SetMinInputs( 2 );
	SetMaxInputs( 2 );

	// Parameters
	// The name here must match the one you declared in your fragment shader.
	// Resolume will look for a param named "Opacity" for mix value.
	AddParam( Param::Create( "Opacity" ) );

	FFGLLog::LogToHost( "Created Add mixer" );
}
Add::~Add()
{
}

FFResult Add::InitGL( const FFGLViewportStruct* vp )
{
	if( !shader.Compile( _vertexShaderCode, _fragmentShaderCode ) )
	{
		DeInitGL();
		return FF_FAIL;
	}
	if( !quad.Initialise() )
	{
		DeInitGL();
		return FF_FAIL;
	}

	//Use base-class init as success result so that it retains the viewport.
	return CFFGLPlugin::InitGL( vp );
}
FFResult Add::ProcessOpenGL( ProcessOpenGLStruct* pGL )
{
	if( pGL->numInputTextures < 2 )
		return FF_FAIL;

	if( pGL->inputTextures[ 0 ] == nullptr )
		return FF_FAIL;
	if( pGL->inputTextures[ 1 ] == nullptr )
		return FF_FAIL;

	//Activate our shader using the scoped binding so that we'll restore the context state when we're done.
	ScopedShaderBinding shaderBinding( shader.GetGLID() );
	//The shader's samplers are fixed so we need to bind the texture to these exact sampler indices. Use the scoped
	//bindings to ensure that the context will be returned in it's default state after we're done rendering.
	ScopedSamplerActivation activateSampler0( 0 );
	Scoped2DTextureBinding textureBinding0( pGL->inputTextures[ 0 ]->Handle );
	ScopedSamplerActivation activateSampler1( 1 );
	Scoped2DTextureBinding textureBinding1( pGL->inputTextures[ 1 ]->Handle );
	shader.Set( "textureDest", 0 );
	shader.Set( "textureSrc", 1 );

	//The input texture's dimension might change each frame and so might the content area.
	//We're adopting the texture's maxUV using a uniform because that way we dont have to update our vertex buffer each frame.
	FFGLTexCoords maxCoords = GetMaxGLTexCoords( *pGL->inputTextures[ 0 ] );
	shader.Set( "MaxUVDest", maxCoords.s, maxCoords.t );
	maxCoords = GetMaxGLTexCoords( *pGL->inputTextures[ 1 ] );
	shader.Set( "MaxUVSrc", maxCoords.s, maxCoords.t );

	//This takes care of sending all the parameter that the plugin registered to the shader.
	SendParams( shader );

	//Apply our shader to the full screen quad.
	quad.Draw();

	return FF_SUCCESS;
}
FFResult Add::DeInitGL()
{
	shader.FreeGLResources();
	quad.Release();
	return FF_SUCCESS;
}
