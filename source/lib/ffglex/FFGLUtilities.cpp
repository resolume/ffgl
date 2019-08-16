#include "FFGLUtilities.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <Carbon/Carbon.h>
#endif

#include <math.h>
#include <random>
#include <iostream>
#include <algorithm>

namespace ffglex
{
unsigned int is_power_of_2( unsigned int x )
{
	return ( x != 0 ) && ( ( x & ( x - 1 ) ) == 0 );
}

int npot( int n )
{
	if( is_power_of_2( n ) )
		return n;

	int prevn = n;

	while( n &= n - 1 )
		prevn = n;
	return prevn * 2;
}

void HSVtoRGB( float h, float s, float v, float& r, float& g, float& b )
{
	if( s == 0 )
	{
		r = v;
		g = v;
		b = v;
	}
	else
	{
		float var_h = h * 6;
		float var_i  = floor( var_h );
		float var_1  = v * ( 1 - s );
		float var_2  = v * ( 1 - s * ( var_h - var_i ) );
		float var_3  = v * ( 1 - s * ( 1 - ( var_h - var_i ) ) );

		if( var_i == 0 )
		{
			r = v;
			g = var_3;
			b = var_1;
		}
		else if( var_i == 1 )
		{
			r = var_2;
			g = v;
			b = var_1;
		}
		else if( var_i == 2 )
		{
			r = var_1;
			g = v;
			b = var_3;
		}
		else if( var_i == 3 )
		{
			r = var_1;
			g = var_2;
			b = v;
		}
		else if( var_i == 4 )
		{
			r = var_3;
			g = var_1;
			b = v;
		}
		else
		{
			r = v;
			g = var_1;
			b = var_2;
		}
	}
}
void HSVtoRGB( double h, double s, double v, double* r, double* g, double* b )
{
	if( s == 0 )
	{
		*r = v;
		*g = v;
		*b = v;
	}
	else
	{
		double var_h = h * 6;
		double var_i = floor( var_h );
		double var_1 = v * ( 1 - s );
		double var_2 = v * ( 1 - s * ( var_h - var_i ) );
		double var_3 = v * ( 1 - s * ( 1 - ( var_h - var_i ) ) );

		if( var_i == 0 )
		{
			*r = v;
			*g = var_3;
			*b = var_1;
		}
		else if( var_i == 1 )
		{
			*r = var_2;
			*g = v;
			*b = var_1;
		}
		else if( var_i == 2 )
		{
			*r = var_1;
			*g = v;
			*b = var_3;
		}
		else if( var_i == 3 )
		{
			*r = var_1;
			*g = var_2;
			*b = v;
		}
		else if( var_i == 4 )
		{
			*r = var_3;
			*g = var_1;
			*b = v;
		}
		else
		{
			*r = v;
			*g = var_1;
			*b = var_2;
		}
	}
}

float random( float min, float max )
{
	return min + ( rand() / ( RAND_MAX / ( max - min ) ) );
}
float clamp01( float value )
{
	return std::min( std::max( value, 0.0f ), 1.0f );
}
float clamp( float value, float low, float high )
{
	return std::max( std::min( value, high ), low );
}
float map( float value, float low, float high, float newLow, float newHigh )
{
	float res = newLow + ( value - low ) * ( newHigh - newLow ) / ( high - low );
	return clamp( res, newLow, newHigh );
}

void ReplaceAll( std::string& utf8String, const std::string& valueToReplace, const std::string& replaceWith )
{
	if( valueToReplace == replaceWith )
		return;
	for( std::string::size_type offset = utf8String.find( valueToReplace ); offset != std::string::npos; offset = utf8String.find( valueToReplace, offset ) )
		utf8String.replace( offset, valueToReplace.length(), replaceWith );
}
void Log( const std::string& message )
{
#if defined( _WIN32 )
	OutputDebugString( message.c_str() );
	OutputDebugString( "\n" );
	std::cout << message << std::endl;
#else
	printf( "%s", (message + "\n").c_str() );
#endif
}
}//End namespace ffglex