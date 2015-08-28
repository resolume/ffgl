/*
 *  utilities.cpp
 *  FFGLPlugins
 *
 *  Created by Edwin de Koning on 12/13/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */


#include "utilities.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <Carbon/Carbon.h>
#endif

#include <Math.h>


#ifdef _WIN32
typedef __int64  int64;
inline int64 abs64 (const int64 n) throw()                      { return (n >= 0) ? n : -n; }

static int64 hiResTicksPerSecond = -1;
static double hiResTicksScaleFactor = 0.0;
#endif

double getTicks()
{
	
	#ifdef _WIN32
	//return (double) GetTickCount();
    
	//snippet for timing taken from JUCE library
	if (hiResTicksPerSecond == -1) {
		LARGE_INTEGER f;
		QueryPerformanceFrequency (&f);
		hiResTicksPerSecond = f.QuadPart;
		hiResTicksScaleFactor = 1000.0 / hiResTicksPerSecond;
	}

	LARGE_INTEGER ticks;
    QueryPerformanceCounter (&ticks);

    const int64 mainCounterAsHiResTicks = (GetTickCount() * hiResTicksPerSecond) / 1000;
    const int64 newOffset = mainCounterAsHiResTicks - ticks.QuadPart;

    // fix for a very obscure PCI hardware bug that can make the counter
    // sometimes jump forwards by a few seconds..
    static int64 hiResTicksOffset = 0;
    const int64 offsetDrift = abs64 (newOffset - hiResTicksOffset);

    if (offsetDrift > (hiResTicksPerSecond >> 1))
        hiResTicksOffset = newOffset;

    return ((double) (ticks.QuadPart + hiResTicksOffset)) * hiResTicksScaleFactor;

	#else

	Nanoseconds nano = AbsoluteToNanoseconds( UpTime() );
	// if you want that in (floating point) seconds:
	double seconds = ((double) UnsignedWideToUInt64( nano )) * 1e-9;
	return seconds * 1000.0;

	
	#endif

}

unsigned int is_power_of_2(unsigned int x)
{
	return (x != 0) && ((x & (x - 1)) == 0);
}


int npot(int n)
{
	if (is_power_of_2(n)) return n;

	int prevn = n;

	while(n &= n-1)
        prevn = n;
    return prevn * 2;

} 

void HSVtoRGB(double h, double s, double v, double* r, double* g, double* b)
{

  if ( s == 0 )

  {

     *r = v;

     *g = v;

     *b = v;

  } else {

     double var_h = h * 6;

     double var_i = floor( var_h );
     
	 double var_1 = v * ( 1 - s );
     
	 double var_2 = v * ( 1 - s * ( var_h - var_i ) );
     
	 double var_3 = v * ( 1 - s * ( 1 - ( var_h - var_i ) ) );
 

     if      ( var_i == 0 ) { *r = v     ; *g = var_3 ; *b = var_1; }

     else if ( var_i == 1 ) { *r = var_2 ; *g = v     ; *b = var_1; }

     else if ( var_i == 2 ) { *r = var_1 ; *g = v     ; *b = var_3; }

     else if ( var_i == 3 ) { *r = var_1 ; *g = var_2 ; *b = v;     }

     else if ( var_i == 4 ) { *r = var_3 ; *g = var_1 ; *b = v;     }

     else                   { *r = v     ; *g = var_1 ; *b = var_2; }

 

  }

}



