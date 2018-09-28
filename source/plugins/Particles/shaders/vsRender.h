#pragma once

static const char vsRender[] = R"(#version 410 core
#line 5
uniform float MAX_AGE;
uniform float FADEOUT_START;
uniform float SMOKE_START;
uniform float SMOKE_INTENSITY;
uniform vec4 BUCKET_COLORS[ MAX_BUCKETS ];
uniform float BURST_DURATION;

layout( location = 0 ) in vec2 vPosition;    //Particle position
layout( location = 1 ) in vec2 vVelocity;    //Particle velocity
layout( location = 2 ) in vec2 vBurstVel;    //Burst velocity, fully faded out after 25% life.
layout( location = 3 ) in float vAge;        //Age
layout( location = 4 ) in int vBucketIndex;  //Bucket Index

out vec2 particlePosition;
out vec4 particleColor;
out vec2 particleVelocity;

void main()
{
	float agePhase = clamp( vAge / MAX_AGE, 0.0, 1.0 );

	particlePosition = vPosition;
	particleVelocity = (vVelocity + (1.0 - clamp( vAge / MAX_AGE, 0.0, BURST_DURATION ) / BURST_DURATION) * vBurstVel);

	gl_Position = vec4( particlePosition.xy, 0.0, 1.0 );
	particleColor = BUCKET_COLORS[ vBucketIndex ];
	if( agePhase > FADEOUT_START )
		particleColor.a = 1.0 - smoothstep( 0.0, 1.0, (agePhase - FADEOUT_START) / (1.0 - FADEOUT_START) );
	if( agePhase > SMOKE_START )
		particleColor.rgb = mix( particleColor.rgb, vec3( SMOKE_INTENSITY ), smoothstep( 0.0, 1.0, (agePhase - SMOKE_START) / (1.0 - SMOKE_START) ) );
}
)";
