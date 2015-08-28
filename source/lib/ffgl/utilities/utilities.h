/*
 *  utilities.h
 *  FFGLPlugins
 *
 *  Created by Edwin de Koning on 12/13/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once 

#include "FFGL.h"

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
    {  1.0f, -1.0f, 1.0f },
    {  1.0f,  1.0f, 1.0f },
    { -1.0f,  1.0f, 1.0f }
};

static GlVertexTextured g_QuadVerticesTextured[] = 
{
    { 0.0f, 0.0f, -1.0f,-1.0f, 0.0f },
    { 1.0f, 0.0f,  1.0f,-1.0f, 0.0f },
    { 1.0f, 1.0f,  1.0f, 1.0f, 0.0f },
    { 0.0f, 1.0f, -1.0f, 1.0f, 0.0f }
};


static GlVertex g_cubeVertices[] =
{
    { -1.0f,-1.0f, 1.0f },
    {  1.0f,-1.0f, 1.0f },
    {  1.0f, 1.0f, 1.0f },
    { -1.0f, 1.0f, 1.0f },
   
    { -1.0f,-1.0f,-1.0f },
    { -1.0f, 1.0f,-1.0f },
    {  1.0f, 1.0f,-1.0f },
    {  1.0f,-1.0f,-1.0f },
   
    { -1.0f, 1.0f,-1.0f },
    { -1.0f, 1.0f, 1.0f },
    {  1.0f, 1.0f, 1.0f },
    {  1.0f, 1.0f,-1.0f },
   
    { -1.0f,-1.0f,-1.0f },
    {  1.0f,-1.0f,-1.0f },
    {  1.0f,-1.0f, 1.0f },
    { -1.0f,-1.0f, 1.0f },
   
    { 1.0f,-1.0f,-1.0f },
    { 1.0f, 1.0f,-1.0f },
    { 1.0f, 1.0f, 1.0f },
    { 1.0f,-1.0f, 1.0f },
   
    { -1.0f,-1.0f,-1.0f },
    { -1.0f,-1.0f, 1.0f },
    { -1.0f, 1.0f, 1.0f },
    { -1.0f, 1.0f,-1.0f }
};


static GlVertexTextured g_cubeVerticesTextured[] =
{
    { 0.0f,0.0f, -1.0f,-1.0f, 1.0f },
    { 1.0f,0.0f,  1.0f,-1.0f, 1.0f },
    { 1.0f,1.0f,  1.0f, 1.0f, 1.0f },
    { 0.0f,1.0f, -1.0f, 1.0f, 1.0f },
   
    { 1.0f,0.0f, -1.0f,-1.0f,-1.0f },
    { 1.0f,1.0f, -1.0f, 1.0f,-1.0f },
    { 0.0f,1.0f,  1.0f, 1.0f,-1.0f },
    { 0.0f,0.0f,  1.0f,-1.0f,-1.0f },
   
    { 0.0f,1.0f, -1.0f, 1.0f,-1.0f },
    { 0.0f,0.0f, -1.0f, 1.0f, 1.0f },
    { 1.0f,0.0f,  1.0f, 1.0f, 1.0f },
    { 1.0f,1.0f,  1.0f, 1.0f,-1.0f },
   
    { 1.0f,1.0f, -1.0f,-1.0f,-1.0f },
    { 0.0f,1.0f,  1.0f,-1.0f,-1.0f },
    { 0.0f,0.0f,  1.0f,-1.0f, 1.0f },
    { 1.0f,0.0f, -1.0f,-1.0f, 1.0f },
   
    { 1.0f,0.0f,  1.0f,-1.0f,-1.0f },
    { 1.0f,1.0f,  1.0f, 1.0f,-1.0f },
    { 0.0f,1.0f,  1.0f, 1.0f, 1.0f },
    { 0.0f,0.0f,  1.0f,-1.0f, 1.0f },
   
    { 0.0f,0.0f, -1.0f,-1.0f,-1.0f },
    { 1.0f,0.0f, -1.0f,-1.0f, 1.0f },
    { 1.0f,1.0f, -1.0f, 1.0f, 1.0f },
    { 0.0f,1.0f, -1.0f, 1.0f,-1.0f }
};

static GlVertex g_cubeVerticesWired[] =
{
	//Front face
	{ -1.0f, -1.0f,  1.0f},
    {  1.0f, -1.0f,  1.0f},
    {  1.0f,  1.0f,  1.0f},
    { -1.0f,  1.0f,  1.0f},
    { -1.0f, -1.0f,  1.0f},

    // Back Face
    { -1.0f, -1.0f, -1.0f},
    { -1.0f,  1.0f, -1.0f},
    {  1.0f,  1.0f, -1.0f},
    {  1.0f, -1.0f, -1.0f},
    { -1.0f, -1.0f, -1.0f},

    // Top Face
    { -1.0f,  1.0f, -1.0f},
    { -1.0f,  1.0f,  1.0f},
    {  1.0f,  1.0f,  1.0f},
    {  1.0f,  1.0f, -1.0f},
    { -1.0f,  1.0f, -1.0f},

    // Bottom Face
    { -1.0f, -1.0f, -1.0f},
    {  1.0f, -1.0f, -1.0f},
    {  1.0f, -1.0f,  1.0f},
    { -1.0f, -1.0f,  1.0f},
    { -1.0f, -1.0f, -1.0f},

    // Right face
    {  1.0f, -1.0f, -1.0f},
    {  1.0f,  1.0f, -1.0f},
    {  1.0f,  1.0f,  1.0f},
    {  1.0f, -1.0f,  1.0f},
    {  1.0f, -1.0f, -1.0f},

    // Left Face
    { -1.0f, -1.0f, -1.0f},
    { -1.0f, -1.0f,  1.0f},
    { -1.0f,  1.0f,  1.0f},
    { -1.0f,  1.0f, -1.0f},
    { -1.0f, -1.0f, -1.0f}


};

#define STRINGIFY( expr ) std::string(#expr)

unsigned int is_power_of_2(unsigned int x);
int npot(int n);

double getTicks();
void HSVtoRGB(double h, double s, double v, double* r, double* g, double* b);