#include "Constants.h"
#include <FFGLSDK.h>
using namespace ffglex;

float* GetBucketColors()
{
	static float BUCKET_COLORS[ MAX_BUCKETS ][ 4 ];
	static bool colorsInited = false;
	if( !colorsInited )
	{
		colorsInited = true;
		for( size_t index = 0; index < MAX_BUCKETS; ++index )
		{
			HSVtoRGB( index / float( MAX_BUCKETS - 1 ) * 2.0f / 3.0f, 1.0f, 1.0f, BUCKET_COLORS[ index ][ 0 ], BUCKET_COLORS[ index ][ 1 ], BUCKET_COLORS[ index ][ 2 ] );
			BUCKET_COLORS[ index ][ 3 ] = 1.0f;
		}
	}

	return (float*)BUCKET_COLORS;
}