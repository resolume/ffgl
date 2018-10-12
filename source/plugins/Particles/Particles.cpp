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

//Just using an enum to give our Parameter ID's unique numbers.
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
	PID_BURST_INTENSITY,

	PID_SIMULATE,
	PID_FFT_INPUT
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
	burstIntensity( 4.0f ),
	simulate( true )
{
	// Input properties
	SetMinInputs( 0 );
	SetMaxInputs( 0 );

	//Register our params using the new utility function that gets the current value and uses it as the default.
	SetParamInfof( PID_FADEOUT_START, "Fadeout Start", FF_TYPE_STANDARD );
	SetParamInfof( PID_SMOKE_START, "Smoke Start", FF_TYPE_STANDARD );
	SetParamInfof( PID_SMOKE_INTENSITY, "Smoke Intensity", FF_TYPE_STANDARD );
	SetParamInfof( PID_ENERGY_MAX_AGE_FACTOR, "Energy To Age Factor", FF_TYPE_STANDARD );
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
	SetParamInfo( PID_SIMULATE, "Simulate", FF_TYPE_BOOLEAN, simulate );

	SetBufferParamInfo( PID_FFT_INPUT, "FFT", MAX_BUCKETS, FF_USAGE_FFT );
	for( unsigned int index = 0; index < MAX_BUCKETS; ++index )
		SetParamElementInfo( PID_FFT_INPUT, index, "", 0.1f ); //Default to 0.1 so that we'll keep emitting even if the host doesn't provide fft data.
}

FFResult Particles::InitGL( const FFGLViewportStruct* vp )
{
	if( !glResources.Initialise() )
	{
		DeInitGL();
		return FF_FAIL;
	}

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
	float timeNow = getTicks() / 1000.0f;
	float deltaTime = timeNow - lastUpdate;
	lastUpdate = timeNow;

	if( simulate )
		UpdateParticles( deltaTime );
	RenderParticles();

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

	case PID_SIMULATE:
		simulate = value != 0.0f;
		break;
	case PID_FFT_INPUT:  //This case is here to keep the ffgl framework happy. By responding with FF_SUCCESS we're telling it that this is a valid param id.
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

	case PID_SIMULATE:
		return simulate ? 1.0f : 0.0f;
	case PID_FFT_INPUT:
		return 0.0f;

	default:
		return 0.0f;
	}
}

void Particles::UpdateParticles( float deltaTime )
{
	std::vector< float > fftData( MAX_BUCKETS );
	const ParamInfo* fftInfo = FindParamInfo( PID_FFT_INPUT );
	for( size_t index = 0; index < MAX_BUCKETS; ++index )
		fftData[ index ] = fftInfo->elements[ index ].value;

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
		spawnArea.t = -1.0f;
		spawnArea.b = -1.1f;
		//left, width, bottom, height
		spawnAreas[ index ] = Vec4f( spawnArea.l, spawnArea.getWidth(), spawnArea.t, spawnArea.getHeight() );
		spawnChances[ index ] = fftData[ index ] * fftData[ index ] * 3.0f;
	}
	glUniform1f( glResources.GetUpdateShader().FindUniform( "MAX_AGE" ), 60.0f / bpm * maxAge );
	glUniform4fv( glResources.GetUpdateShader().FindUniform( "SPAWN_AREAS" ), (GLsizei)spawnAreas.size(), (float*)spawnAreas.data() );
	glUniform1fv( glResources.GetUpdateShader().FindUniform( "SPAWN_CHANCES" ), (GLsizei)spawnChances.size(), spawnChances.data() );
	glUniform1f( glResources.GetUpdateShader().FindUniform( "ENERGY_MAX_AGE_FACTOR" ), energyMaxAgeFactor );
	glUniform1f( glResources.GetUpdateShader().FindUniform( "TURBULENCE_DETAIL" ), turbulenceDetail );
	glUniform1f( glResources.GetUpdateShader().FindUniform( "TURBULENCE_SPEED" ), turbulenceSpeed );
	glUniform1f( glResources.GetUpdateShader().FindUniform( "BURST_DURATION" ), burstDuration );
	glUniform1f( glResources.GetUpdateShader().FindUniform( "BURST_INTENSITY" ), burstIntensity );

	glUniform1f( glResources.GetUpdateShader().FindUniform( "Time" ), getTicks() / 1000.0f );
	glUniform1f( glResources.GetUpdateShader().FindUniform( "DeltaTime" ), deltaTime );
	glUniform2i( glResources.GetUpdateShader().FindUniform( "RenderSize" ), currentViewport.width, currentViewport.height );

	ScopedVAOBinding vaoBinding( glResources.GetFrontVAOID() );

	glDrawArrays( GL_POINTS, 0, static_cast< GLsizei >( numBuckets * numParticlesPerBucket ) );

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
	Scoped2DTextureBinding textureBinding( glResources.GetParticleTextureID() );

	glUniform1f( glResources.GetRenderShader().FindUniform( "MAX_AGE" ), 60.0f / bpm * maxAge );
	glUniform1f( glResources.GetRenderShader().FindUniform( "FADEOUT_START" ), fadeoutStart );
	glUniform1f( glResources.GetRenderShader().FindUniform( "SMOKE_START" ), smokeStart );
	glUniform1f( glResources.GetRenderShader().FindUniform( "SMOKE_INTENSITY" ), smokeIntensity );
	glUniform1i( glResources.GetRenderShader().FindUniform( "ParticleTexture" ), 0 );
	glUniform1f( glResources.GetRenderShader().FindUniform( "ParticleSize" ), particleSize );
	glUniform1f( glResources.GetRenderShader().FindUniform( "VelocitySizeFactor" ), velocityToSizeFactor );
	glUniform4fv( glResources.GetRenderShader().FindUniform( "BUCKET_COLORS" ), MAX_BUCKETS, GetBucketColors() );
	glUniform2f( glResources.GetRenderShader().FindUniform( "MAX_UV" ), 1.0f, 1.0f );
	glUniform1f( glResources.GetRenderShader().FindUniform( "BURST_DURATION" ), burstDuration );

	glUniform2i( glResources.GetRenderShader().FindUniform( "RenderSize" ), currentViewport.width, currentViewport.height );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	glDrawArrays( GL_POINTS, 0, static_cast< GLsizei >( numBuckets * numParticlesPerBucket ) );
	//FFGL requires us to keep the entire context state at default, this includes the blendmode, so set it back.
	glBlendFunc( GL_ONE, GL_ZERO );
	glDisable( GL_BLEND );
}