#include "AddSubtract.h"
using namespace ffglex;

#define FFPARAM_BrightnessR ( 0 )
#define FFPARAM_BrightnessG ( 1 )
#define FFPARAM_BrightnessB ( 2 )

static CFFGLPluginInfo PluginInfo(
	PluginFactory< AddSubtract >,// Create method
	"RE01",                      // Plugin unique ID of maximum length 4.
	"AddSub Example",            // Plugin name
	2,                           // API major version number
	0,                         // API minor version number
	1,                           // Plugin major version number
	000,                         // Plugin minor version number
	FF_EFFECT,                   // Plugin type
	"Add and Subtract colours",  // Plugin description
	"Resolume FFGL Example"      // About
);

static const char vertexShaderCode[] = R"(#version 330
uniform vec2 MaxUV;

layout( location = 0 ) in vec4 vPosition;
layout( location = 1 ) in vec2 vUV;

out vec2 uv;

void main()
{
	gl_Position = vPosition;
	uv = vUV * MaxUV;
}
)";

static const char fragmentShaderCode[] = R"(#version 330
uniform sampler2D InputTexture;
uniform vec3 Brightness;

in vec2 uv;

out vec4 fragColor;

void main()
{
	vec4 color = texture( InputTexture, uv );
	//The InputTexture contains premultiplied colors, so we need to unpremultiply first to apply our effect on straight colors.
	if( color.a > 0.0 )
		color.rgb /= color.a;

	color.rgb = color.rgb + Brightness;

	//The plugin has to output premultiplied colors, this is how we're premultiplying our straight color while also
	//ensuring we aren't going out of the LDR the video engine is working in.
	color.rgb = clamp( color.rgb * color.a, vec3( 0.0 ), vec3( color.a ) );
	fragColor = color;
}
)";

AddSubtract::AddSubtract() :
	maxUVLocation( -1 ),
	brightnessLocation( -1 )
{
	// Input properties
	SetMinInputs( 1 );
	SetMaxInputs( 1 );

	// Parameters
	SetParamInfo( FFPARAM_BrightnessR, "R", FF_TYPE_RED, 0.5f );
	brightnessR = 0.5f;

	SetParamInfo( FFPARAM_BrightnessG, "G", FF_TYPE_GREEN, 0.5f );
	brightnessG = 0.5f;

	SetParamInfo( FFPARAM_BrightnessB, "B", FF_TYPE_BLUE, 0.5f );
	brightnessB = 0.5f;
}
AddSubtract::~AddSubtract()
{
}

FFResult AddSubtract::InitGL( const FFGLViewportStruct* vp )
{
	glGenVertexArrays( 1, &vaoID );
	glGenBuffers( 1, &vboID );

	ScopedVAOBinding vaoBinding( vaoID );
	ScopedVBOBinding vboBinding( vboID );
	glBufferData( GL_ARRAY_BUFFER, sizeof( TEXTURED_QUAD_VERTICES ), TEXTURED_QUAD_VERTICES, GL_STATIC_DRAW );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, false, sizeof( TEXTURED_QUAD_VERTICES[ 0 ] ), (char*)NULL + 2 * sizeof( float ) );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, false, sizeof( TEXTURED_QUAD_VERTICES[ 0 ] ), (char*)NULL + 0 * sizeof( float ) );

	vaoBinding.EndScope();

	glDisableVertexAttribArray( 0 );
	glDisableVertexAttribArray( 1 );

	shader.Compile( vertexShaderCode, fragmentShaderCode );

	ScopedShaderBinding shaderBinding( shader.GetGLID() );
	//We're never changing the sampler to use, instead during rendering we'll make sure that we're always
	//binding the texture to sampler 0.
	glUniform1i( shader.FindUniform( "inputTexture" ), 0 );

	//We need to know these uniform locations because we need to set their value each frame.
	maxUVLocation = shader.FindUniform( "MaxUV" );
	brightnessLocation = shader.FindUniform( "brightness" );

	if( vaoID == 0 || vboID == 0 || !shader.IsReady() )
	{
		DeInitGL();
		return FF_FAIL;
	}
	return FF_SUCCESS;
}
FFResult AddSubtract::DeInitGL()
{
	shader.FreeGLResources();
	maxUVLocation = -1;
	brightnessLocation = -1;
	glDeleteBuffers( 1, &vboID );
	vboID = 0;
	glDeleteVertexArrays( 1, &vaoID );
	vaoID = 0;

	return FF_SUCCESS;
}

FFResult AddSubtract::ProcessOpenGL( ProcessOpenGLStruct* pGL )
{
	if( pGL->numInputTextures < 1 )
		return FF_FAIL;

	if( pGL->inputTextures[ 0 ] == NULL )
		return FF_FAIL;

	ScopedVAOBinding vaoBinding( vaoID );
	ScopedShaderBinding shaderBinding( shader.GetGLID() );

	FFGLTextureStruct& Texture = *( pGL->inputTextures[ 0 ] );

	//The input texture's dimension might change each frame and so might the content area.
	//We're adopting the texture's maxUV using a uniform because that way we dont have to update our vertex buffer each frame.
	FFGLTexCoords maxCoords = GetMaxGLTexCoords( Texture );
	glUniform2f( maxUVLocation, maxCoords.s, maxCoords.t );

	glUniform3f( brightnessLocation,
				 -1.0f + ( brightnessR * 2.0f ),
				 -1.0f + ( brightnessG * 2.0f ),
				 -1.0f + ( brightnessB * 2.0f ) );

	//The shader's sampler is always bound to sampler index 0 so that's where we need to bind the texture.
	ScopedSamplerActivation activateSampler( 0 );
	Scoped2DTextureBinding textureBinding( Texture.Handle );

	glDrawArrays( GL_TRIANGLES, 0, 6 );

	return FF_SUCCESS;
}

FFResult AddSubtract::SetFloatParameter( unsigned int dwIndex, float value )
{
	switch( dwIndex )
	{
	case FFPARAM_BrightnessR:
		brightnessR = value;
		break;
	case FFPARAM_BrightnessG:
		brightnessG = value;
		break;
	case FFPARAM_BrightnessB:
		brightnessB = value;
		break;
	default:
		return FF_FAIL;
	}

	return FF_SUCCESS;
}

float AddSubtract::GetFloatParameter( unsigned int dwIndex )
{
	float retValue = 0.0;

	switch( dwIndex )
	{
	case FFPARAM_BrightnessR:
		retValue = brightnessR;
		return retValue;
	case FFPARAM_BrightnessG:
		retValue = brightnessG;
		return retValue;
	case FFPARAM_BrightnessB:
		retValue = brightnessB;
		return retValue;

	default:
		return retValue;
	}
}
