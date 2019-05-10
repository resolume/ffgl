#include "Bloom.h"
#include <cmath>

using namespace ffglex;

static PluginInstance p = Effect::createPlugin< Bloom >( {
	"FL13",// plugin unique ID
	"Bloom"// Plugin name
} );

static const std::string thresholdShader = R"(
uniform vec2 texelSize;

vec3 Median(vec3 a, vec3 b, vec3 c)
{
    return a + b + c - min(min(a, b), c) - max(max(a, b), c);
}

void main()
{
	vec3 d = texelSize.xyx * vec3(1, 1, 0);
    vec4 s0 = texture(inputTexture, i_uv);
    vec3 s1 = texture(inputTexture, i_uv - d.xz).rgb;
    vec3 s2 = texture(inputTexture, i_uv + d.xz).rgb;
    vec3 s3 = texture(inputTexture, i_uv - d.zy).rgb;
    vec3 s4 = texture(inputTexture, i_uv + d.zy).rgb;
	vec3 col = Median(Median(s0.rgb, s1, s2), s3, s4);
	float brightness = max(max(col.r, col.g), col.b);
	col.rgb = mix(vec3(0), col.rgb, smoothstep(threshold-.05,threshold, brightness));
	fragColor = vec4(col.rgb, s0.a);
}
)";

// Downsample with a 4x4 box filter
static const std::string downsampleFilterShader = R"(
#version 410 core
out vec4 fragColor;

in vec2 i_uv;
uniform vec2 texelSize;
uniform sampler2D inputTexture;
uniform bool antiFlicker;

float Brightness(vec4 c)
{
    return max(max(c.r, c.g), c.b);
}

vec4 getDownSampleAntiFlicker() {
	vec4 d = texelSize.xyxy * vec4(-1, -1, +1, +1);
	vec4 s1 = texture( inputTexture, i_uv + d.xy );
	vec4 s2 = texture( inputTexture, i_uv + d.zy );
	vec4 s3 = texture( inputTexture, i_uv + d.xw );
	vec4 s4 = texture( inputTexture, i_uv + d.zw );

	// Karis's luma weighted average (using brightness instead of luma)
    float s1w = 1 / (Brightness(s1) + 1);
    float s2w = 1 / (Brightness(s2) + 1);
    float s3w = 1 / (Brightness(s3) + 1);
    float s4w = 1 / (Brightness(s4) + 1);
    float one_div_wsum = 1 / (s1w + s2w + s3w + s4w);

	return (s1 * s1w + s2 * s2w + s3 * s3w + s4 * s4w) * one_div_wsum;
}

vec4 getDownSample() {
	vec4 d = texelSize.xyxy * vec4(-1, -1, +1, +1);
	vec4 col = texture( inputTexture, i_uv + d.xy );
	col += texture( inputTexture, i_uv + d.zy );
	col += texture( inputTexture, i_uv + d.xw );
	col += texture( inputTexture, i_uv + d.zw );
	return col / 4.;
}

void main()
{
	vec4 col;
	if (antiFlicker) {
		col = getDownSampleAntiFlicker();
	} else {
		col = getDownSample();
	}
	fragColor = vec4(col);
}
)";

// 4-tap bilinear upsampler
static const std::string upsampleFilterShader = R"(
#version 410 core
out vec4 fragColor;

uniform sampler2D inputTexture;
uniform sampler2D baseTexture;

in vec2 i_uv;
uniform float sampleScale;
uniform vec2 texelSize;
uniform bool hq;

vec4 getUpSample(vec2 uv) {
	vec4 d = texelSize.xyxy * vec4(-1, -1, +1, +1) * sampleScale * 0.5;
	vec4 col = texture( inputTexture, uv + d.xy );
	col += texture( inputTexture, uv + d.zy );
	col += texture( inputTexture, uv + d.xw );
	col += texture( inputTexture, uv + d.zw );
	return col / 4.;
}

vec4 getUpSampleHighQ(vec2 uv) {
	vec4 d = texelSize.xyxy * vec4(1, 1, -1, 0) * sampleScale;
    vec4 col = texture( inputTexture, uv - d.xy );
    col += texture( inputTexture, uv - d.wy ) * 2;
    col += texture( inputTexture, uv - d.zy );

    col += texture( inputTexture, uv + d.zw ) * 2;
    col += texture( inputTexture, uv        ) * 4;
    col += texture( inputTexture, uv + d.xw ) * 2;

    col += texture( inputTexture, uv + d.zy );
    col += texture( inputTexture, uv + d.wy ) * 2;
    col += texture( inputTexture, uv + d.xy );

	return col * (1.0 / 16);
}

void main()
{
	vec4 base = texture( baseTexture, i_uv );
	vec4 col;
	if (hq) {
		col = getUpSampleHighQ(i_uv);
	} else {
		col = getUpSample(i_uv);
	}
	fragColor = vec4(base.rgb + col.rgb,base.a);
}
)";

static const std::string finalShader = R"(
#version 410 core
out vec4 fragColor;

uniform sampler2D inputTexture;
uniform sampler2D baseTexture;

in vec2 i_uv;
uniform vec2 maxUV;
uniform float intensity;
uniform float sampleScale;
uniform vec2 texelSize;
uniform bool hq;

vec4 getUpSample(vec2 uv) {
	vec4 d = texelSize.xyxy * vec4(-1, -1, +1, +1) * sampleScale * 0.5;
	vec4 col = texture( inputTexture, uv + d.xy );
	col += texture( inputTexture, uv + d.zy );
	col += texture( inputTexture, uv + d.xw );
	col += texture( inputTexture, uv + d.zw );
	return col / 4.;
}

vec4 getUpSampleHighQ(vec2 uv) {
	vec4 d = texelSize.xyxy * vec4(1, 1, -1, 0) * sampleScale;
    vec4 col = texture( inputTexture, uv - d.xy );
    col += texture( inputTexture, uv - d.wy ) * 2;
    col += texture( inputTexture, uv - d.zy );

    col += texture( inputTexture, uv + d.zw ) * 2;
    col += texture( inputTexture, uv        ) * 4;
    col += texture( inputTexture, uv + d.xw ) * 2;

    col += texture( inputTexture, uv + d.zy );
    col += texture( inputTexture, uv + d.wy ) * 2;
    col += texture( inputTexture, uv + d.xy );

	return col * (1.0 / 16);
}

void main()
{
	vec4 base = texture( baseTexture, i_uv );
	vec4 d = texelSize.xyxy * vec4(-1, -1, +1, +1) * sampleScale * 0.5;
	vec2 uv = i_uv / maxUV ;
	vec4 col;
	if (hq) {
		col = getUpSampleHighQ(uv);
	} else {
		col = getUpSample(uv);
	}
	vec3 outColor = base.rgb + col.rgb * intensity;
	fragColor = vec4( outColor, base.a);
}
)";

Bloom::Bloom()
{
	setFragmentShader( thresholdShader );
	addParam( threshold = ParamRange::create( "threshold", 0.8f, { 0, 1 } ) );
	addParam( radius = ParamRange::create( "radius", 2.5f, { 1, 7 } ) );
	addParam( intensity = ParamRange::create( "intensity", 0.8f, { 0, 1 } ) );
	addParam( hq = ParamBool::create( "hq" ) );
	addParam( antiFlicker = ParamBool::create( "antiFlicker" ) );
}

FFResult Bloom::init()
{
	if( !downSampleFilter.Compile( vertexShaderCode, downsampleFilterShader ) )
	{
		DeInitGL();
		return FF_FAIL;
	}
	if( !upSampleFilter.Compile( vertexShaderCode, upsampleFilterShader ) )
	{
		DeInitGL();
		return FF_FAIL;
	}
	if( !final.Compile( vertexShaderCode, finalShader ) )
	{
		DeInitGL();
		return FF_FAIL;
	}
	return FF_SUCCESS;
}

FFResult Bloom::render( ProcessOpenGLStruct* inputTextures )
{
	static const int kMaxIterations = 16;

	// determine the iteration count
	double logh        = std::log( currentViewport.height ) / std::log( 2 ) + radius->getRealValue() - 8;
	int logh_i        = (int)logh;
	int iterations     = (int)utils::clamp( (float)logh_i, 1.0f, (float)kMaxIterations );
	double sampleScale = 0.5f + logh - logh_i;

	FFGLFBO thresholdFBO;
	FFGLFBO mipmaps[ kMaxIterations ];
	FFGLFBO combine[ kMaxIterations ];

	// Prefilter pixel above a certain brightness threshold
	thresholdFBO.Create( currentViewport.width, currentViewport.height, GL_RGBA16F );
	thresholdFBO.BindAsRenderTarget();
	FFGLFBO* last = &thresholdFBO;
	shader.Use();
	shader.Set( "texelSize", 1.0f / (float)last->GetWidth(), 1.0f / (float)last->GetHeight() );
	FFResult result = Effect::render( inputTextures );
	if( result == FF_FAIL )
		return FF_FAIL;

	// Create a mipmap pyramid
	downSampleFilter.Use();
	downSampleFilter.Set( "maxUV", 1.f, 1.f );
	downSampleFilter.Set( "antiFlicker", (int)antiFlicker->getValue() );
	for( int i = 0; i < iterations; i++ )
	{
		mipmaps[ i ].Create( last->GetWidth() / 2, last->GetHeight() / 2, GL_RGBA16F );
		mipmaps[ i ].BindAsRenderTarget();
		mipmaps[ i ].ResizeViewPort();
		downSampleFilter.Set( "texelSize", 1.0f / (float)last->GetWidth(), 1.0f / (float)last->GetHeight() );
		downSampleFilter.Bind( "inputTexture", 0, last->GetTextureInfo() );
		quad.Draw();
		last = &mipmaps[ i ];
	}

	// upsample and combine loop
	upSampleFilter.Use();
	upSampleFilter.Set( "maxUV", 1.f, 1.f );
	upSampleFilter.Set( "hq", (int)hq->getValue() );
	for( int i = iterations - 2; i >= 0; i-- )
	{
		combine[ i ].Create( mipmaps[ i ].GetWidth(), mipmaps[ i ].GetHeight(), GL_RGBA16F );
		combine[ i ].BindAsRenderTarget();
		combine[ i ].ResizeViewPort();
		upSampleFilter.Set( "texelSize", 1.0f / (float)last->GetWidth(), 1.0f / (float)last->GetHeight() );
		upSampleFilter.Set( "sampleScale", (float)sampleScale );
		upSampleFilter.Bind( "inputTexture", 0, last->GetTextureInfo() );
		upSampleFilter.Bind( "baseTexture", 1, mipmaps[ i ].GetTextureInfo() );
		quad.Draw();
		last = &combine[ i ];
	}

	glBindFramebuffer( GL_FRAMEBUFFER, inputTextures->HostFBO );
	glViewport( 0, 0, currentViewport.width, currentViewport.height );
	final.Use();
	final.Set( "hq", (int)hq->getValue() );
	FFGLTexCoords maxCoords = GetMaxGLTexCoords( *inputTextures->inputTextures[ 0 ] );
	final.Set( "maxUV", maxCoords.s, maxCoords.t );
	final.Bind( "inputTexture", 0, last->GetTextureInfo() );
	final.Bind( "baseTexture", 1, *inputTextures->inputTextures[ 0 ] );
	final.Set( "intensity", intensity->getRealValue() );
	final.Set( "texelSize", 1.0f / (float)last->GetWidth(), 1.0f / (float)last->GetHeight() );
	final.Set( "sampleScale", (float)sampleScale );
	quad.Draw();

	// Free all allocated ressource
	for( int i = 0; i < iterations; i++ )
	{
		mipmaps[ i ].FreeResources();
		combine[ i ].FreeResources();
	}
	thresholdFBO.FreeResources();
	return FF_SUCCESS;
}

void Bloom::clean()
{
	downSampleFilter.FreeGLResources();
	upSampleFilter.FreeGLResources();
	final.FreeGLResources();
}

Bloom::~Bloom()
{
}