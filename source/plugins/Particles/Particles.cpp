#include "Particles.h"
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Constants.h"
using namespace ffglex;

static CFFGLPluginInfo PluginInfo(
	PluginFactory< Particles >,    // Create method
	"RS02",                        // Plugin unique ID
	"Particles Example",           // Plugin name
	2,                             // API major version number
	1,                             // API minor version number
	1,                             // Plugin major version number
	0,                             // Plugin minor version number
	FF_SOURCE,                     // Plugin type
	"Sample FFGL Particles plugin",// Plugin description
	"Resolume FFGL Example"        // About
);

struct Vec4f
{
	Vec4f() :
		x( 0.0f ), y( 0.0f ), z( 0.0f ), w( 0.0f )
	{
	}
	Vec4f( float x, float y, float z, float w ) :
		x( x ), y( y ), z( z ), w( w )
	{
	}

	float x, y, z, w;
};
struct Rectf
{
	float getWidth()  const
	{
		if( r > l )
			return r - l;
		else
			return l - r;
	}
	float getHeight()  const
	{
		if( t > b )
			return t - b;
		else
			return b - t;
	}

	float l, r, b, t;
};

Particles::Particles()
{
	// Input properties
	SetMinInputs( 0 );
	SetMaxInputs( 0 );

	//Register our params using the new utility function that gets the current value and uses it as the default.
	addParam( fadeoutStart = ParamRange::create("Fadeout Start", 0.2f, { 0.0f, 1.0f } ) );
	addParam( smokeStart = ParamRange::create( "Smoke Start", 0.5f, { 0.0f, 1.0f } ) );
	addParam( smokeIntensity = ParamRange::create( "Smoke Intensity", 0.4f, { 0.0f, 1.0f } ) );
	addParam( energyMaxAgeFactor = ParamRange::create( "Energy To Age Factor", 0.3f, { 0.0f, 1.0f } ) );
	addParam( turbulenceDetail = ParamRange::create( "Turbulence Detail", 1.2f, { 0.5f, 1.5f } ) );
	addParam( turbulenceSpeed = ParamRange::create( "Turbulence Speed", 0.5f, { 0.1f, 2.0f } ) );
	addParam( maxAge = ParamOption::create( "Max Age", {
		{ "0.5 beats", 0.5f },
		{ "1 beat", 1.0f },
		{ "2 beats", 2.0f },
		{ "4 beats", 4.0f },
		{ "16 beats", 16.0f }
	}, 3));
	addParam( particleSize = ParamRange::create( "Particle Size", 1.0f, { 1.0f, 10.0f } ) );
	addParam( velocityToSizeFactor = ParamRange::create( "Velocity Size Factor", 7.0f, { 1.0f, 20.0f } ) );
	addParam( numBuckets = ParamRange::create( "Num Buckets", 1.0f, { 16.0f, (float) MAX_BUCKETS } ) );
	addParam( numParticlesPerBucket = ParamRange::create( "Num Particles Per Bucket", 1.0f, { 0.0f, (float) MAX_PARTICLES_PER_BUCKET } ) );
	addParam( burstDuration = ParamRange::create( "Burst Duration", 1.0f, { 0.0f, 1.0f } ) );
	addParam( burstIntensity = ParamRange::create( "Burst Intensity", 1.0f, { 1.0f, 16.0f } ) );
	addParam( simulate = ParamBool::create( "simulate", true ) );
	addParam( fft = ParamFFT::create( "FFT", MAX_BUCKETS ) );

	for( unsigned int element_index = 0; element_index < MAX_BUCKETS; ++element_index )
		SetParamElementInfo( fft->index, element_index, "", 0.1f );//Default to 0.1 so that we'll keep emitting even if the host doesn't provide fft data.
}

FFResult Particles::InitGL( const FFGLViewportStruct* vp )
{
	if( !glResources.Initialise() )
	{
		DeInitGL();
		return FF_FAIL;
	}

	//FFGL requires us to leave the context in a default state on return
	resetOpenGLState();
	//Use base-class init as success result so that it retains the viewport.
	return CFreeFrameGLPlugin::InitGL( vp );
}
FFResult Particles::DeInitGL()
{
	glResources.Release();

	return FF_SUCCESS;
}
FFResult Particles::ProcessOpenGL( ProcessOpenGLStruct* pGL )
{
	if( simulate->getValue() )
		UpdateParticles( deltaTime );
	RenderParticles();

	//FFGL requires us to leave the context in a default state on return
	resetOpenGLState();
	return FF_SUCCESS;
}

void Particles::UpdateParticles( float deltaTime )
{
	updateAudioAndTime();
	glResources.FlipBuffers();

	glResources.GetUpdateShader().Use();
	glEnable( GL_RASTERIZER_DISCARD );
	glBindBufferRange( GL_TRANSFORM_FEEDBACK_BUFFER, 0, glResources.GetBackBufferID(), 0, MAX_BUCKETS * MAX_PARTICLES_PER_BUCKET * sizeof( Vertex ) );
	glBeginTransformFeedback( GL_POINTS );

	float numBucketsVal = numBuckets->getRealValue();
	std::vector< Vec4f > spawnAreas( (size_t)numBucketsVal );
	std::vector< float > spawnChances( (size_t)numBucketsVal );
	memset( spawnChances.data(), 0, spawnChances.size() * sizeof( float ) );
	//TODO: Map from fft's num bins to our num buckets. Audio guys halp!
	for( size_t index = 0; index < numBucketsVal && index < fft->fftData.size(); ++index )
	{
		float widthPerBucket = 1.6f / numBucketsVal;
		Rectf spawnArea;
		spawnArea.l = -0.8f + index * widthPerBucket + 0.1f * widthPerBucket;
		spawnArea.r = spawnArea.l + widthPerBucket - 0.1f * widthPerBucket;
		spawnArea.t = -1.0f;
		spawnArea.b = -1.1f;
		//left, width, bottom, height
		spawnAreas[ index ] = Vec4f( spawnArea.l, spawnArea.getWidth(), spawnArea.t, spawnArea.getHeight() );
		spawnChances[ index ] = fft->fftData[ index ] * fft->fftData[ index ] * 3.0f;
	}
	glResources.GetUpdateShader().Set( "MAX_AGE", 60.0f / bpm * maxAge->getRealValue() );
	glUniform4fv( glResources.GetUpdateShader().FindUniform( "SPAWN_AREAS" ), (GLsizei)spawnAreas.size(), (float*)spawnAreas.data() );
	glUniform1fv( glResources.GetUpdateShader().FindUniform( "SPAWN_CHANCES" ), (GLsizei)spawnChances.size(), spawnChances.data() );
	glResources.GetUpdateShader().Set( "ENERGY_MAX_AGE_FACTOR", energyMaxAgeFactor->getRealValue() );
	glResources.GetUpdateShader().Set( "TURBULENCE_DETAIL", turbulenceDetail->getRealValue() );
	glResources.GetUpdateShader().Set( "TURBULENCE_SPEED", turbulenceSpeed->getRealValue() );
	glResources.GetUpdateShader().Set( "BURST_DURATION", burstDuration->getRealValue() );
	glResources.GetUpdateShader().Set( "BURST_INTENSITY", burstIntensity->getRealValue() );

	glResources.GetUpdateShader().Set( "Time", timeNow );
	glResources.GetUpdateShader().Set( "DeltaTime", deltaTime );
	glResources.GetUpdateShader().Set( "RenderSize", (float)currentViewport.width, (float)currentViewport.height );

	glBindVertexArray( glResources.GetFrontVAOID() );

	glDrawArrays( GL_POINTS, 0, static_cast< GLsizei >( numBucketsVal * numParticlesPerBucket->getRealValue() ) );

	glBindVertexArray( 0 );

	glEndTransformFeedback();
	glBindBufferRange( GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0, 0, 0 );
	glDisable( GL_RASTERIZER_DISCARD );
}
void Particles::RenderParticles()
{
	glResources.GetRenderShader().Use();
	glBindVertexArray( glResources.GetBackVAOID() );
	ScopedSamplerActivation samplerBinding( 0 );
	Scoped2DTextureBinding textureBinding( glResources.GetParticleTextureID() );

	glResources.GetRenderShader().Set( "MAX_AGE", 60.0f / bpm * maxAge->getRealValue() );
	glResources.GetRenderShader().Set( "FADEOUT_START", fadeoutStart->getRealValue() );
	glResources.GetRenderShader().Set( "SMOKE_START", smokeStart->getRealValue() );
	glResources.GetRenderShader().Set( "SMOKE_INTENSITY", smokeIntensity->getRealValue() );
	glResources.GetRenderShader().Set( "ParticleTexture", 0 );
	glResources.GetRenderShader().Set( "ParticleSize", particleSize->getRealValue() );
	glResources.GetRenderShader().Set( "VelocitySizeFactor", velocityToSizeFactor->getRealValue() );
	glUniform4fv( glResources.GetRenderShader().FindUniform( "BUCKET_COLORS"), MAX_BUCKETS, GetBucketColors() );
	glResources.GetRenderShader().Set( "MAX_UV", 1.0f, 1.0f);
	glResources.GetRenderShader().Set( "BURST_DURATION", burstDuration->getRealValue() );

	glResources.GetRenderShader().Set( "RenderSize", (float)currentViewport.width, (float)currentViewport.height );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	glDrawArrays( GL_POINTS, 0, static_cast< GLsizei >( numBuckets->getRealValue() * numParticlesPerBucket->getRealValue() ) );
	//FFGL requires us to keep the entire context state at default, this includes the blendmode, so set it back.
	glBlendFunc( GL_ONE, GL_ZERO );
	glDisable( GL_BLEND );
}