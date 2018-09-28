#pragma once

static const char vsDebugRender[] = R"(#version 410 core
#line 5
uniform vec4 BUCKET_COLORS[ MAX_BUCKETS ];

layout( location = 0 ) in vec2 vPosition;    //Particle position
layout( location = 1 ) in vec2 vVelocity;    //Particle velocity
layout( location = 2 ) in vec2 vBurstVel;    //Burst velocity, fully faded out after 25% life.
layout( location = 3 ) in float vAge;        //Age
layout( location = 4 ) in int vBucketIndex;  //Bucket Index

out vec4 color;
out int bucketIndex;

void main()
{
	vec2 particlePosition = vPosition * vec2( 1.0, -1.0 );

	gl_Position = vec4( particlePosition.xy, 0.0, 1.0 );

	color = BUCKET_COLORS[ vBucketIndex ];
	//color = vec4( vBucketIndex / float( MAX_BUCKETS ) );//BUCKET_COLORS[ vBucketIndex ];
	bucketIndex = vBucketIndex;
}
)";
