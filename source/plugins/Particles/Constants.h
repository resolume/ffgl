#pragma once

static const int MAX_BUCKETS              = 64;
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