#include "GLResources.h"
#include <vector>
#include <cmath>
#include <assert.h>
#include "Constants.h"
#include "shaders/vsUpdate.h"
#include "shaders/vsRender.h"
#include "shaders/gsRender.h"
#include "shaders/fsRender.h"
using namespace ffglex;

//#define USE_DEBUG_INIT_DATA 1

GLResources::GLResources() :
	particleTextureID( 0 ),
	frontIndex( 0 )
{
	vaoIDs[ 0 ] = vaoIDs[ 1 ] = 0;
	vboIDs[ 0 ] = vboIDs[ 1 ] = 0;
	updateShader.AddTransformFeedbackVarying( "oPosition" );
	updateShader.AddTransformFeedbackVarying( "oVelocity" );
	updateShader.AddTransformFeedbackVarying( "oBurstVel" );
	updateShader.AddTransformFeedbackVarying( "oAge" );
	updateShader.AddTransformFeedbackVarying( "oBucketIndex" );
}
GLResources::~GLResources()
{
	assert( particleTextureID == 0 );
	assert( vaoIDs[ 0 ] == 0 && vaoIDs[ 1 ] == 0 );
	assert( vboIDs[ 0 ] == 0 && vboIDs[ 1 ] == 0 );
}
bool GLResources::Initialise()
{
	if( !LoadParticleTexture() )
	{
		Release();
		return false;
	}
	if( !LoadVertexBuffers() )
	{
		Release();
		return false;
	}
	if( !LoadShaders() )
	{
		Release();
		return false;
	}

	return true;
}
void GLResources::Release()
{
	glDeleteTextures( 1, &particleTextureID );
	particleTextureID = 0;
	glDeleteVertexArrays( 2, vaoIDs );
	vaoIDs[ 0 ] = vaoIDs[ 1 ] = 0;
	glDeleteBuffers( 2, vboIDs );
	vboIDs[ 0 ] = vboIDs[ 1 ] = 0;
	updateShader.FreeGLResources();
	renderShader.FreeGLResources();
}

void GLResources::FlipBuffers()
{
	frontIndex = (frontIndex + 1) % 2;
}

GLuint GLResources::GetParticleTextureID() const
{
	return particleTextureID;
}
GLuint GLResources::GetFrontVAOID() const
{
	return vaoIDs[ frontIndex ];
}
GLuint GLResources::GetBackVAOID() const
{
	return vaoIDs[ (frontIndex + 1) % 2 ];
}
GLuint GLResources::GetFrontBufferID() const
{
	return vboIDs[ frontIndex ];
}
GLuint GLResources::GetBackBufferID() const
{
	return vboIDs[ (frontIndex + 1) % 2 ];
}
const FFGLShader& GLResources::GetUpdateShader() const
{
	return updateShader;
}
const FFGLShader& GLResources::GetRenderShader() const
{
	return renderShader;
}

bool GLResources::LoadParticleTexture()
{
	std::vector< unsigned char > pixels( 16 * 16 );
	for( size_t y = 0; y < 15; ++y )
	{
		for( size_t x = 0; x < 15; ++x )
		{
			float deltaY         = y - 7.0f;
			float deltaX         = x - 7.0f;
			pixels[ y * 15 + x ] = static_cast< unsigned char >( powf( clamp01( 1.0f - sqrt( deltaX * deltaX + deltaY * deltaY ) / 8.0f ), 0.16f ) * 255.0f );
		}
	}

	glGenTextures( 1, &particleTextureID );
	if( particleTextureID == 0 )
		return false;

	//Use the scoped binding so that the context state is restored to it's default as required by ffgl.
	Scoped2DTextureBinding textureBinding( particleTextureID );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	//We've been lazy with our pixel building above, tell the driver that our data isn't 4byte aligned (which is context default)
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_R8, 15, 15, 0, GL_RED, GL_UNSIGNED_BYTE, pixels.data() );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );//Set it back to the context's default state as required by ffgl.
	return true;
}
bool GLResources::LoadVertexBuffers()
{
	std::vector< Vertex > vertexData( MAX_BUCKETS * MAX_PARTICLES_PER_BUCKET );
	memset( vertexData.data(), 0, vertexData.size() * sizeof( Vertex ) );

	/**
	 * Evenly distribute particle indices between buckets. We're using vertex id (it's index) as input for randomization,
	 * so we need every bucket to contain both high and low vertex indices rather than all low indices in the first bucket
	 * and high indices into the last. Using this distribution provides us with a more uniform randomized behaviour when using vertex id.
	 * This distribution also enables us to reduce particle count by just updating/rendering less without hitting just the last few bins.
	 * Ignoring particles from the end hits all buckets evenly.
	 */
	size_t vertexIndex = 0;
	for( int particleIndex = 0; particleIndex < MAX_PARTICLES_PER_BUCKET; ++particleIndex )
	{
		for( int bucketIndex = 0; bucketIndex < MAX_BUCKETS; ++bucketIndex )
		{
			Vertex& vertex           = vertexData[ vertexIndex ];
			vertex.age               = std::numeric_limits< float >::max();
			vertex.bucketIndex       = bucketIndex;

#if defined( USE_DEBUG_INIT_DATA )
			vertex.age           = particleIndex / float( MAX_PARTICLES_PER_BUCKET - 1 ) * 16.0f;
			float widthPerBucket = 2.0f / MAX_BUCKETS;
			vertex.position[ 0 ] = -1.0f + bucketIndex * widthPerBucket + ( rand() / float( RAND_MAX ) ) * widthPerBucket;
			vertex.position[ 1 ] = -1.0f + ( rand() / float( RAND_MAX ) ) * 2.0f;
			vertex.velocity[ 1 ] = 2.0f / 16.0f * 0.8f;
#endif
			vertexIndex++;
		}
	}

	glGenVertexArrays( 2, vaoIDs );
	if( vaoIDs[ 0 ] == 0 || vaoIDs[ 1 ] == 0 )
		return false;

	glGenBuffers( 2, vboIDs );
	if( vboIDs[ 0 ] == 0 || vboIDs[ 1 ] == 0 )
		return false;

	//FFGL requires us to leave the context in a default state on return, so use these scoped bindings to help us do that.
	ScopedVAOBinding vaoBinding0( vaoIDs[ 0 ] );
	ScopedVBOBinding vboBinding0( vboIDs[ 0 ] );
	glBufferData( GL_ARRAY_BUFFER, MAX_BUCKETS * MAX_PARTICLES_PER_BUCKET * sizeof( Vertex ), vertexData.data(), GL_STATIC_DRAW );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 2, GL_FLOAT, false, sizeof( Vertex ), (char*)NULL + 0 * sizeof( float ) );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, false, sizeof( Vertex ), (char*)NULL + 2 * sizeof( float ) );
	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 2, GL_FLOAT, false, sizeof( Vertex ), (char*)NULL + 4 * sizeof( float ) );
	glEnableVertexAttribArray( 3 );
	glVertexAttribPointer( 3, 1, GL_FLOAT, false, sizeof( Vertex ), (char*)NULL + 6 * sizeof( float ) );
	glEnableVertexAttribArray( 4 );
	glVertexAttribIPointer( 4, 1, GL_INT, sizeof( Vertex ), (char*)NULL + 7 * sizeof( float ) );

	//Manually end the scope so that we wont do two unbindings when we're done.
	vboBinding0.EndScope();
	vaoBinding0.EndScope();

	//FFGL requires us to leave the context in a default state on return, so use these scoped bindings to help us do that.
	ScopedVAOBinding vaoBinding1( vaoIDs[ 1 ] );
	ScopedVBOBinding vboBinding1( vboIDs[ 1 ] );
	glBufferData( GL_ARRAY_BUFFER, MAX_BUCKETS * MAX_PARTICLES_PER_BUCKET * sizeof( Vertex ), vertexData.data(), GL_STATIC_DRAW );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 2, GL_FLOAT, false, sizeof( Vertex ), (char*)NULL + 0 * sizeof( float ) );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, false, sizeof( Vertex ), (char*)NULL + 2 * sizeof( float ) );
	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 2, GL_FLOAT, false, sizeof( Vertex ), (char*)NULL + 4 * sizeof( float ) );
	glEnableVertexAttribArray( 3 );
	glVertexAttribPointer( 3, 1, GL_FLOAT, false, sizeof( Vertex ), (char*)NULL + 6 * sizeof( float ) );
	glEnableVertexAttribArray( 4 );
	glVertexAttribIPointer( 4, 1, GL_INT, sizeof( Vertex ), (char*)NULL + 7 * sizeof( float ) );

	vboBinding1.EndScope();
	vaoBinding1.EndScope();

	return true;
}
bool GLResources::LoadShaders()
{
	std::string vsUpdateShader( vsUpdate );
	ReplaceAll( vsUpdateShader, "MAX_BUCKETS", std::to_string( MAX_BUCKETS ) );
	std::string vsRenderShader( vsRender );
	ReplaceAll( vsRenderShader, "MAX_BUCKETS", std::to_string( MAX_BUCKETS ) );

	if( !updateShader.Compile( vsUpdateShader.c_str(), nullptr ) )
		return false;
	if( !renderShader.Compile( vsRenderShader.c_str(), gsRender, fsRender ) )
		return false;

	return true;
}
