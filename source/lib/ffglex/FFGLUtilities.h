#pragma once
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

#define STRINGIFY( expr ) std::string( #expr )

unsigned int is_power_of_2( unsigned int x );
int npot( int n );

double getTicks();
void HSVtoRGB( double h, double s, double v, double* r, double* g, double* b );

float random( float min, float max );

}//End namespace ffglex