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
	0,                             // API minor version number
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

enum ParamID
{
	PID_FADEOUT_START,
	PID_SMOKE_START,
	PID_SMOKE_INTENSITY,
	PID_ENERGY_MAX_AGE_FACTOR,
	PID_TURBULENCE_DETAIL,
	PID_TURBULENCE_SPEED,
	PID_MAX_AGE,
	PID_PARTICLE_SIZE,
	PID_VELOCITY_SIZE_FACTOR,
	PID_NUM_BUCKETS,
	PID_NUM_PARTICLES_PER_BUCKET,
	PID_BURST_DURATION,
	PID_BURST_INTENSITY
};

Particles::Particles() :
	fadeoutStart( 0.2f ),
	smokeStart( 0.5f ),
	smokeIntensity( 0.4f ),
	energyMaxAgeFactor( 0.3f ),
	turbulenceDetail( 1.2f ),
	turbulenceSpeed( 0.5f ),
	maxAge( 4.0f ),
	particleSize( 1.0f ),
	velocityToSizeFactor( 7.0f ),
	numBuckets( 32 ),
	numParticlesPerBucket( int( MAX_PARTICLES_PER_BUCKET * 0.5f ) ),
	burstDuration( 0.25f ),
	burstIntensity( 4.0f )
{
	// Input properties
	SetMinInputs( 0 );
	SetMaxInputs( 0 );

	SetParamInfof( PID_FADEOUT_START, "Fadeout Start", FF_TYPE_STANDARD );
	SetParamInfof( PID_SMOKE_START, "Smoke Start", FF_TYPE_STANDARD );
	SetParamInfof( PID_SMOKE_INTENSITY, "Smoke Intensity", FF_TYPE_STANDARD );
	SetParamInfof( PID_SMOKE_INTENSITY, "Energy To Age Factor", FF_TYPE_STANDARD );
	SetParamInfof( PID_TURBULENCE_DETAIL, "Turbulence Detail", FF_TYPE_STANDARD );
	SetParamInfof( PID_TURBULENCE_SPEED, "Turbulence Speed", FF_TYPE_STANDARD );
	SetOptionParamInfo( PID_MAX_AGE, "Max Age", 5, maxAge );
	SetParamElementInfo( PID_MAX_AGE, 0, "0.5 beats", 0.5f );
	SetParamElementInfo( PID_MAX_AGE, 1, "1 beat", 1.0f );
	SetParamElementInfo( PID_MAX_AGE, 2, "2 beats", 2.0f );
	SetParamElementInfo( PID_MAX_AGE, 3, "4 beats", maxAge );
	SetParamElementInfo( PID_MAX_AGE, 4, "16 beats", 16.0f );
	SetParamInfof( PID_PARTICLE_SIZE, "Particle Size", FF_TYPE_STANDARD );
	SetParamInfof( PID_VELOCITY_SIZE_FACTOR, "Velocity Size Factor", FF_TYPE_STANDARD );
	SetParamInfof( PID_NUM_BUCKETS, "Num Buckets", FF_TYPE_STANDARD );
	SetParamInfof( PID_NUM_PARTICLES_PER_BUCKET, "Num Particles Per Bucket", FF_TYPE_STANDARD );
	SetParamInfof( PID_BURST_DURATION, "Burst Duration", FF_TYPE_STANDARD );
	SetParamInfof( PID_BURST_INTENSITY, "Burst Intensity", FF_TYPE_STANDARD );
}

FFResult Particles::InitGL( const FFGLViewportStruct* vp )
{
	if( !glResources.Initialise() )
	{
		DeInitGL();
		return FF_FAIL;
	}

	return FF_SUCCESS;
}
FFResult Particles::DeInitGL()
{
	glResources.Release();

	return FF_SUCCESS;
}
FFResult Particles::ProcessOpenGL( ProcessOpenGLStruct* pGL )
{
	UpdateParticles();
	RenderParticles();

	/*
	ScopedShaderBinding shaderBinding( glResources.GetDebugRenderShader().GetGLID() );
	ScopedVAOBinding vaoBinding( glResources.GetFrontVAOID() );

	glUniform4fv( glResources.GetDebugRenderShader().FindUniform( "BUCKET_COLORS" ), MAX_BUCKETS, GetBucketColors() );
	glDrawArrays( GL_POINTS, 0, MAX_BUCKETS * MAX_PARTICLES_PER_BUCKET );
	*/
	return FF_SUCCESS;
}

FFResult Particles::SetFloatParameter( unsigned int dwIndex, float value )
{
	switch( dwIndex )
	{
	case PID_FADEOUT_START:
		fadeoutStart = value;
		break;
	case PID_SMOKE_START:
		smokeStart = value;
		break;
	case PID_SMOKE_INTENSITY:
		smokeIntensity = value;
		break;
	case PID_ENERGY_MAX_AGE_FACTOR:
		energyMaxAgeFactor = value;
		break;
	case PID_TURBULENCE_DETAIL:
		turbulenceDetail = value + 0.5f;
		break;
	case PID_TURBULENCE_SPEED:
		turbulenceSpeed = value * 1.9f + 0.1f;
		break;
	case PID_MAX_AGE:
		maxAge = value;
		break;
	case PID_PARTICLE_SIZE:
		particleSize = value * 9.0f + 1.0f;
		break;
	case PID_VELOCITY_SIZE_FACTOR:
		velocityToSizeFactor = value * 19.0f + 1.0f;
		break;
	case PID_NUM_BUCKETS:
		numBuckets = static_cast< int >( value * (MAX_BUCKETS - 16.0f) + 16.0f );
		break;
	case PID_NUM_PARTICLES_PER_BUCKET:
		numParticlesPerBucket = static_cast< int >( value * MAX_PARTICLES_PER_BUCKET );
		break;
	case PID_BURST_DURATION:
		burstDuration = value;
		break;
	case PID_BURST_INTENSITY:
		burstIntensity = value * 15.0f + 1.0f;
		break;

	default:
		return FF_FAIL;
	}

	return FF_SUCCESS;
}

float Particles::GetFloatParameter( unsigned int index )
{
	switch( index )
	{
	case PID_FADEOUT_START:
		return fadeoutStart;
	case PID_SMOKE_START:
		return smokeStart;
	case PID_SMOKE_INTENSITY:
		return smokeIntensity;
	case PID_ENERGY_MAX_AGE_FACTOR:
		return energyMaxAgeFactor;
	case PID_TURBULENCE_DETAIL:
		return turbulenceDetail - 0.5f;
	case PID_TURBULENCE_SPEED:
		return (turbulenceSpeed - 0.1f) / 1.9f;
	case PID_MAX_AGE:
		return maxAge;
	case PID_PARTICLE_SIZE:
		return (particleSize - 1.0f) / 9.0f;
	case PID_VELOCITY_SIZE_FACTOR:
		return (velocityToSizeFactor - 1.0f) / 19.0f;
	case PID_NUM_BUCKETS:
		return (numBuckets - 16.0f) / (MAX_BUCKETS - 16.0f);
	case PID_NUM_PARTICLES_PER_BUCKET:
		return static_cast< float >( numParticlesPerBucket ) / MAX_PARTICLES_PER_BUCKET;
	case PID_BURST_DURATION:
		return burstDuration;
	case PID_BURST_INTENSITY:
		return (burstIntensity - 1.0f) / 15.0f;

	default:
		return 0.0f;
	}
}

void Particles::UpdateParticles()
{
	std::vector< float > fftData( 64 );
	//std::fill( fftData.begin(), fftData.end(), 0.4f );
	float peakBucket = sinf( getTicks() / 1000.0f * 2.0f ) * 32.0f + 32.0f;
	float phasePerBucket = M_PI * 2.0f / fftData.size();
	for( size_t index = 0; index < fftData.size(); ++index )
		fftData[ index ] = std::max( 0.0f, 0.2f * sinf( M_PI * 0.5f + index * phasePerBucket + peakBucket * phasePerBucket ) );

	glResources.FlipBuffers();

	ScopedShaderBinding shaderBinding( glResources.GetUpdateShader().GetGLID() );

	glEnable( GL_RASTERIZER_DISCARD );
	glBindBufferRange( GL_TRANSFORM_FEEDBACK_BUFFER, 0, glResources.GetBackBufferID(), 0, MAX_BUCKETS * MAX_PARTICLES_PER_BUCKET * sizeof( Vertex ) );
	glBeginTransformFeedback( GL_POINTS );

	std::vector< Vec4f > spawnAreas( numBuckets );
	std::vector< float > spawnChances( numBuckets );
	memset( spawnChances.data(), 0, spawnChances.size() * sizeof( float ) );
	//TODO: Map from fft's num bins to our num buckets. Audio guys halp!
	for( size_t index = 0; index < numBuckets && index < fftData.size(); ++index )
	{
		float widthPerBucket = 1.6f / numBuckets;
		Rectf spawnArea;
		spawnArea.l = -0.8f + index * widthPerBucket + 0.1f * widthPerBucket;
		spawnArea.r = spawnArea.l + widthPerBucket - 0.1f * widthPerBucket;
		spawnArea.b = -1.0f;
		spawnArea.t = -0.9f;
		//left, width, bottom, height
		spawnAreas[ index ] = Vec4f( spawnArea.l, spawnArea.getWidth(), spawnArea.b, spawnArea.getHeight() );
		spawnChances[ index ] = fftData[ index ] * fftData[ index ] * 3.0f;
	}
	glUniform1f( glResources.GetUpdateShader().FindUniform( "MAX_AGE" ), 60.0f / tempo * maxAge );
	glUniform4fv( glResources.GetUpdateShader().FindUniform( "SPAWN_AREAS" ), (GLsizei)spawnAreas.size(), (float*)spawnAreas.data() );
	glUniform1fv( glResources.GetUpdateShader().FindUniform( "SPAWN_CHANCES" ), (GLsizei)spawnChances.size(), spawnChances.data() );
	glUniform1f( glResources.GetUpdateShader().FindUniform( "ENERGY_MAX_AGE_FACTOR" ), energyMaxAgeFactor );
	glUniform1f( glResources.GetUpdateShader().FindUniform( "TURBULENCE_DETAIL" ), turbulenceDetail );
	glUniform1f( glResources.GetUpdateShader().FindUniform( "TURBULENCE_SPEED" ), turbulenceSpeed );
	glUniform1f( glResources.GetUpdateShader().FindUniform( "BURST_DURATION" ), burstDuration );
	glUniform1f( glResources.GetUpdateShader().FindUniform( "BURST_INTENSITY" ), burstIntensity );

	glUniform1f( glResources.GetUpdateShader().FindUniform( "Time" ), getTicks() / 1000.0f );
	glUniform1f( glResources.GetUpdateShader().FindUniform( "DeltaTime" ), 0.017f );
	glUniform2i( glResources.GetUpdateShader().FindUniform( "RenderSize" ), 1920, 1080 );

	ScopedVAOBinding vaoBinding( glResources.GetFrontVAOID() );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	glDrawArrays( GL_POINTS, 0, static_cast< GLsizei >( numBuckets * numParticlesPerBucket ) );
	glBlendFunc( GL_ONE, GL_ZERO );
	glDisable( GL_BLEND );

	vaoBinding.EndScope();

	glEndTransformFeedback();
	glBindBufferRange( GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0, 0, 0 );
	glDisable( GL_RASTERIZER_DISCARD );
}
void Particles::RenderParticles()
{
	ScopedShaderBinding shaderBinding( glResources.GetRenderShader().GetGLID() );
	ScopedVAOBinding vaoBinding( glResources.GetBackVAOID() );
	ScopedSamplerActivation samplerBinding( 0 );
	Scoped2DTextureBinding( glResources.GetParticleTextureID() );

	glUniform1f( glResources.GetRenderShader().FindUniform( "MAX_AGE" ), 60.0f / tempo * maxAge );
	glUniform1f( glResources.GetRenderShader().FindUniform( "FADEOUT_START" ), fadeoutStart );
	glUniform1f( glResources.GetRenderShader().FindUniform( "SMOKE_START" ), smokeStart );
	glUniform1f( glResources.GetRenderShader().FindUniform( "SMOKE_INTENSITY" ), smokeIntensity );
	glUniform1i( glResources.GetRenderShader().FindUniform( "ParticleTexture" ), 0 );
	glUniform1f( glResources.GetRenderShader().FindUniform( "ParticleSize" ), particleSize );
	glUniform1f( glResources.GetRenderShader().FindUniform( "VelocitySizeFactor" ), velocityToSizeFactor );
	glUniform4fv( glResources.GetRenderShader().FindUniform( "BUCKET_COLORS" ), MAX_BUCKETS, GetBucketColors() );
	glUniform2f( glResources.GetRenderShader().FindUniform( "MAX_UV" ), 1.0f, 1.0f );
	glUniform1f( glResources.GetRenderShader().FindUniform( "BURST_DURATION" ), burstDuration );

	glUniform1f( glResources.GetRenderShader().FindUniform( "Time" ), getTicks() / 1000.0f );
	glUniform1f( glResources.GetRenderShader().FindUniform( "DeltaTime" ), 0.017f );
	glUniform2i( glResources.GetRenderShader().FindUniform( "RenderSize" ), 1920, 1080 );

	glDrawArrays( GL_POINTS, 0, static_cast< GLsizei >( numBuckets * numParticlesPerBucket ) );
}