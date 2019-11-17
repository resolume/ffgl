#pragma once
#include <string>
#include <sstream>
#include "../ffgl/FFGL.h"//For OpenGL

namespace ffglex
{
struct GlColor
{
	float r, g, b, a;
};

struct GlVertex
{
	float x, y, z;
};

struct GlVertexTextured
{
	float tu, tv;
	float x, y, z;
};

static GlVertex g_QuadVertices[] =
{
	{ -1.0f, -1.0f, 1.0f },
	{ 1.0f, -1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ -1.0f, 1.0f, 1.0f }
};
static const GlVertexTextured TEXTURED_QUAD_VERTICES[] =
{
	{ 0.0f, 1.0f, -1.0f, 1.0f, 0.0f }, //Top-left
	{ 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },  //Top-right
	{ 0.0f, 0.0f, -1.0f, -1.0f, 0.0f },//Bottom left

	{ 0.0f, 0.0f, -1.0f, -1.0f, 0.0f },//Bottom left
	{ 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },  //Top right
	{ 1.0f, 0.0f, 1.0f, -1.0f, 0.0f }, //Bottom right
};
static GlVertexTextured g_QuadVerticesTextured[] =
{
	{ 0.0f, 0.0f, -1.0f, -1.0f, 0.0f },
	{ 1.0f, 0.0f, 1.0f, -1.0f, 0.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
	{ 0.0f, 1.0f, -1.0f, 1.0f, 0.0f }
};

static GlVertex g_cubeVertices[] =
{
	{ -1.0f, -1.0f, 1.0f },
	{ 1.0f, -1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ -1.0f, 1.0f, 1.0f },

	{ -1.0f, -1.0f, -1.0f },
	{ -1.0f, 1.0f, -1.0f },
	{ 1.0f, 1.0f, -1.0f },
	{ 1.0f, -1.0f, -1.0f },

	{ -1.0f, 1.0f, -1.0f },
	{ -1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, -1.0f },

	{ -1.0f, -1.0f, -1.0f },
	{ 1.0f, -1.0f, -1.0f },
	{ 1.0f, -1.0f, 1.0f },
	{ -1.0f, -1.0f, 1.0f },

	{ 1.0f, -1.0f, -1.0f },
	{ 1.0f, 1.0f, -1.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, -1.0f, 1.0f },

	{ -1.0f, -1.0f, -1.0f },
	{ -1.0f, -1.0f, 1.0f },
	{ -1.0f, 1.0f, 1.0f },
	{ -1.0f, 1.0f, -1.0f }
};

static GlVertexTextured g_cubeVerticesTextured[] =
{
	{ 0.0f, 0.0f, -1.0f, -1.0f, 1.0f },
	{ 1.0f, 0.0f, 1.0f, -1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
	{ 0.0f, 1.0f, -1.0f, 1.0f, 1.0f },

	{ 1.0f, 0.0f, -1.0f, -1.0f, -1.0f },
	{ 1.0f, 1.0f, -1.0f, 1.0f, -1.0f },
	{ 0.0f, 1.0f, 1.0f, 1.0f, -1.0f },
	{ 0.0f, 0.0f, 1.0f, -1.0f, -1.0f },

	{ 0.0f, 1.0f, -1.0f, 1.0f, -1.0f },
	{ 0.0f, 0.0f, -1.0f, 1.0f, 1.0f },
	{ 1.0f, 0.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f, -1.0f },

	{ 1.0f, 1.0f, -1.0f, -1.0f, -1.0f },
	{ 0.0f, 1.0f, 1.0f, -1.0f, -1.0f },
	{ 0.0f, 0.0f, 1.0f, -1.0f, 1.0f },
	{ 1.0f, 0.0f, -1.0f, -1.0f, 1.0f },

	{ 1.0f, 0.0f, 1.0f, -1.0f, -1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f, -1.0f },
	{ 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
	{ 0.0f, 0.0f, 1.0f, -1.0f, 1.0f },

	{ 0.0f, 0.0f, -1.0f, -1.0f, -1.0f },
	{ 1.0f, 0.0f, -1.0f, -1.0f, 1.0f },
	{ 1.0f, 1.0f, -1.0f, 1.0f, 1.0f },
	{ 0.0f, 1.0f, -1.0f, 1.0f, -1.0f }
};

static GlVertex g_cubeVerticesWired[] =
{
	//Front face
	{ -1.0f, -1.0f, 1.0f },
	{ 1.0f, -1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ -1.0f, 1.0f, 1.0f },
	{ -1.0f, -1.0f, 1.0f },

	// Back Face
	{ -1.0f, -1.0f, -1.0f },
	{ -1.0f, 1.0f, -1.0f },
	{ 1.0f, 1.0f, -1.0f },
	{ 1.0f, -1.0f, -1.0f },
	{ -1.0f, -1.0f, -1.0f },

	// Top Face
	{ -1.0f, 1.0f, -1.0f },
	{ -1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, -1.0f },
	{ -1.0f, 1.0f, -1.0f },

	// Bottom Face
	{ -1.0f, -1.0f, -1.0f },
	{ 1.0f, -1.0f, -1.0f },
	{ 1.0f, -1.0f, 1.0f },
	{ -1.0f, -1.0f, 1.0f },
	{ -1.0f, -1.0f, -1.0f },

	// Right face
	{ 1.0f, -1.0f, -1.0f },
	{ 1.0f, 1.0f, -1.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, -1.0f, 1.0f },
	{ 1.0f, -1.0f, -1.0f },

	// Left Face
	{ -1.0f, -1.0f, -1.0f },
	{ -1.0f, -1.0f, 1.0f },
	{ -1.0f, 1.0f, 1.0f },
	{ -1.0f, 1.0f, -1.0f },
	{ -1.0f, -1.0f, -1.0f }
};

unsigned int is_power_of_2( unsigned int x );
int npot( int n );

void HSVtoRGB( float h, float s, float v, float& r, float& g, float& b );
void HSVtoRGB( double h, double s, double v, double* r, double* g, double* b );

float random( float min, float max );
float clamp01( float value );
float clamp( float value, float low, float high );
float map( float value, float low, float high, float newLow, float newHigh );

void ReplaceAll( std::string& utf8String, const std::string& valueToReplace, const std::string& replaceWith );

template< typename T >
void VariadicMessageBuilder( std::ostream& o, T t )
{
	o << t;
}
template< typename T, typename... Args >
void VariadicMessageBuilder( std::ostream& o, T t, const Args& ... args )
{
	VariadicMessageBuilder( o, t );
	VariadicMessageBuilder( o, args... );
}
void Log( const std::string& message );
/**
 * This function logs anything that's streamable to a stringstream, just provide it as many arguments as you want
 * and it'll concatenate all of them into a string and show it in your debugger.
 */
template<typename... Args>
void Log( const Args&... args )
{
	std::ostringstream oss;
	VariadicMessageBuilder( oss, args... );
	Log( oss.str() );
}

}//End namespace ffglex