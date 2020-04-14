#include "GameOfLife.h"
using namespace ffglex;

static CFFGLPluginInfo PluginInfo(
	PluginFactory< GameOfLife >,// Create method
	"CONW",                     // Plugin unique ID of maximum length 4.
	"Game Of Life",              // Plugin name
	2,                          // API major version number
	1,                          // API minor version number
	1,                          // Plugin major version number
	0,                          // Plugin minor version number
	FF_EFFECT,                  // Plugin type
	"Game Of Life",             // Plugin description
	""                          // About
);

static const char _vertexShaderCode[] = R"(#version 410 core


layout( location = 0 ) in vec4 vPosition;
layout( location = 1 ) in vec2 vUV;

out vec2 uv;

void main()
{
	gl_Position = vPosition;
	uv = vUV;
}
)";

static const char _fragmentShaderCode[] = R"(#version 410 core
uniform sampler2D InputTexture;
uniform vec2 Resolution;
uniform bool DoGol;

in vec2 uv;
uniform vec2 MaxUV;

out vec4 fragColor;

void main()
{
	vec2 st = uv * MaxUV;
	vec4 color = texture( InputTexture, st);
	if (DoGol)
	{
		
		vec2 px = vec2( 1.0 / Resolution.x, -1.0 / Resolution.y );
		px *= MaxUV;
	
		vec4 topleft = texture( InputTexture, st + vec2( -px.x, -px.y ));
		vec4 top = texture( InputTexture, st + vec2( 0.0, -px.y) );
		vec4 topright = texture( InputTexture, st + vec2( px.x, -px.y ));
		vec4 left = texture( InputTexture, st + vec2( -px.x, 0.0 ) );
		vec4 center = texture( InputTexture, st  );
		vec4 right = texture( InputTexture, st + vec2( px.x, 0.0 ) );
		vec4 bottomleft = texture( InputTexture, st + vec2( -px.x, px.y ) );
		vec4 bottom = texture( InputTexture, st + vec2( 0.0, px.y ) );
		vec4 bottomright = texture( InputTexture, st + vec2( px.x, px.y ) );

		float Threshold = 0.5f;
		int tl = (topleft.r + topleft.g + topleft.b ) / 3.0 > Threshold ? 1 : 0;
		int t = (top.r + top.g + top.b ) / 3.0 > Threshold ? 1 : 0;
		int tr = (topright.r + topright.g + topright.b ) / 3.0 > Threshold ? 1 : 0;
		int l = (left.r + left.g + left.b ) / 3.0 > Threshold ? 1 : 0;
		int c = (center.r + center.g + center.b ) / 3.0 > Threshold ? 1 : 0;
		int r = (right.r + right.g + right.b ) / 3.0 > Threshold ? 1 : 0;
		int bl = (bottomleft.r + bottomleft.g + bottomleft.b ) / 3.0 > Threshold ? 1 : 0;
		int b = (bottom.r + bottom.g + bottom.b ) / 3.0 > Threshold ? 1 : 0;
		int br = (bottomright.r + bottomright.g + bottomright.b ) / 3.0 > Threshold ? 1 : 0;

		int neighbors = tl + t + tr + l + r + bl + b + br;
		if ( c == 1 && ( neighbors == 2 || neighbors == 3 ) )
			color = vec4(1.0);
		else if ( c == 0 && neighbors == 3 )
			color = vec4(1.0);
		else
			color = vec4(vec3(0.0), 1.0);
		
	}
	fragColor = color;
	
	
}
)";

GameOfLife::GameOfLife() :
	texture( nullptr )
{
	// Input properties
	SetMinInputs( 1 );
	SetMaxInputs( 1 );

	AddParam( sample = ffglqs::ParamEvent::Create( "Sample" ) );
}
GameOfLife::~GameOfLife()
{
}

FFResult GameOfLife::InitGL( const FFGLViewportStruct* vp )
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
FFResult GameOfLife::ProcessOpenGL( ProcessOpenGLStruct* pGL )
{
	if( pGL->numInputTextures < 1 )
		return FF_FAIL;

	if( pGL->inputTextures[ 0 ] == NULL )
		return FF_FAIL;

	//FFGL requires us to leave the context in a default state on return, so use this scoped binding to help us do that.
	ScopedShaderBinding shaderBinding( shader.GetGLID() );
	//The shader's sampler is always bound to sampler index 0 so that's where we need to bind the texture.
	//Again, we're using the scoped bindings to help us keep the context in a default state.
	ScopedSamplerActivation activateSampler( 0 );

	auto inputTex = pGL->inputTextures[ 0 ];

	if( !texture || sample->GetValue() > 0.5 )
	{
		auto textureBinding = Scoped2DTextureBinding( pGL->inputTextures[ 0 ]->Handle );

		shader.Set( "InputTexture", 0 );

		//The input texture's dimension might change each frame and so might the content area.
		//We're adopting the texture's maxUV using a uniform because that way we dont have to update our vertex buffer each frame.
		FFGLTexCoords maxCoords = GetMaxGLTexCoords( *pGL->inputTextures[ 0 ] );
		shader.Set( "MaxUV", maxCoords.s, maxCoords.t );
		shader.Set( "DoGol", false );

		quad.Draw();
	}

	if( sample->GetValue() > 0.5f )
	{
		glDeleteTextures( 0, &handle );

		glGenTextures( 1, &handle );
		Scoped2DTextureBinding textureBinding( handle );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, inputTex->HardwareWidth, inputTex->HardwareHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );

		delete texture;
		texture                 = new FFGLTextureStruct( *inputTex);
		texture->Handle         = handle;

		glCopyTexSubImage2D( GL_TEXTURE_2D,                         //target
							 0,                                     //level
							 0,                                     //xoffset
							 0,                                     //yoffset
							 0,                                     //x
							 0,                                     //y
							 pGL->inputTextures[ 0 ]->HardwareWidth,//width
							 pGL->inputTextures[ 0 ]->HardwareHeight//height
		);
	}

	if( texture )
	{
		Scoped2DTextureBinding textureBinding( texture->Handle );
		shader.Set( "InputTexture", 0 );
		FFGLTexCoords maxCoords = GetMaxGLTexCoords( *texture );
		shader.Set( "MaxUV", maxCoords.s, maxCoords.t );
		shader.Set( "DoGol", true );
		shader.Set( "Resolution", texture->Width, texture->Height );
		

		quad.Draw();

		glCopyTexSubImage2D( GL_TEXTURE_2D,                         //target
							 0,                                     //level
							 0,                                     //xoffset
							 0,                                     //yoffset
							 0,                                     //x
							 0,                                     //y
							 texture->HardwareWidth,                //width
							 texture->HardwareHeight//height
		);
	}

	return FF_SUCCESS;
}
FFResult GameOfLife::DeInitGL()
{
	shader.FreeGLResources();
	quad.Release();

	return FF_SUCCESS;
}
