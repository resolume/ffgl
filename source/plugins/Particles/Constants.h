#pragma once

static const int MAX_BUCKETS = 64; //Resolume supports 64 fft bins so we support up toone bucket per bin
static const int MAX_PARTICLES_PER_BUCKET = 65536;
float* GetBucketColors();

struct Vertex
{
	float position[ 2 ];
	float velocity[ 2 ];
	float burstVel[ 2 ];
	float age;
	int bucketIndex;
};