#pragma once

static const char vsUpdate[] = R"(#version 410 core
#line 5

uniform float MAX_AGE;
uniform float Time;
uniform float DeltaTime;
uniform ivec2 RenderSize;
uniform vec4 SPAWN_AREAS[ MAX_BUCKETS ];       //left, width, bottom, height
uniform float SPAWN_CHANCES[ MAX_BUCKETS ];
uniform float ENERGY_MAX_AGE_FACTOR;
uniform float TURBULENCE_DETAIL;
uniform float TURBULENCE_SPEED;
uniform float BURST_DURATION;
uniform float BURST_INTENSITY;

layout( location = 0 ) in vec2 vPosition;    //Particle position
layout( location = 1 ) in vec2 vVelocity;    //Particle velocity
layout( location = 2 ) in vec2 vBurstVel;    //Burst velocity, fully faded out after 25% life.
layout( location = 3 ) in float vAge;        //Age
layout( location = 4 ) in int vBucketIndex;  //Bucket Index

out vec2 oPosition;
out vec2 oVelocity;
out vec2 oBurstVel;
out float oAge;
out int oBucketIndex;

const float InverseMaxInt = 1.0 / 4294967295.0;
float randhash( uint seed, float b )
{
	uint i = (seed ^ 12345391u) * 2654435769u;
	i ^= (i << 6u) ^ (i >> 26u);
	i *= 2654435769u;
	i += (i << 5u) ^ (i >> 12u);
	return float( b * i ) * InverseMaxInt;
}

vec4 permute(vec4 x){return mod(((x*34.0)+1.0)*x, 289.0);}
vec4 taylorInvSqrt(vec4 r){return 1.79284291400159 - 0.85373472095314 * r;}

float snoise(vec3 v){ 
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

// First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

// Other corners
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  //  x0 = x0 - 0. + 0.0 * C 
  vec3 x1 = x0 - i1 + 1.0 * C.xxx;
  vec3 x2 = x0 - i2 + 2.0 * C.xxx;
  vec3 x3 = x0 - 1. + 3.0 * C.xxx;

// Permutations
  i = mod(i, 289.0 ); 
  vec4 p = permute( permute( permute( 
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

// Gradients
// ( N*N points uniformly over a square, mapped onto an octahedron.)
  float n_ = 1.0/7.0; // N=7
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z *ns.z);  //  mod(p,N*N)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

//Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

// Mix final noise value
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
                                dot(p2,x2), dot(p3,x3) ) );
}

vec2 normalNoise(vec2 _st, float _zoom, float _speed){
	vec2 v1 = _st;
	vec2 v2 = _st;
	vec2 v3 = _st;
	float expon = pow(10.0, _zoom*2.0);
	v1 /= 1.0*expon;
	v2 /= 0.62*expon;
	v3 /= 0.83*expon;
	float n = Time*_speed;
	float nr = (snoise(vec3(v1, n)) + snoise(vec3(v2, n)) + snoise(vec3(v3, n))) / 6.0 + 0.5;
	n = Time * _speed + 1000.0;
	float ng = (snoise(vec3(v1, n)) + snoise(vec3(v2, n)) + snoise(vec3(v3, n))) / 6.0 + 0.5;
	return vec2(nr,ng) * 2.0 - 1.0;
}

void main()
{
	vec2 turbulence = normalNoise( (vPosition * 0.5 + 0.5) * RenderSize, 1.5, TURBULENCE_SPEED ) * 1.0;
	turbulence += normalNoise( (vPosition * 0.5 + 0.5) * RenderSize, TURBULENCE_DETAIL, TURBULENCE_SPEED ) * 2.0;
	turbulence /= RenderSize;
	turbulence *= 50.0;

	oVelocity = vVelocity + turbulence;
	oPosition = vPosition + oVelocity * DeltaTime + (1.0 - clamp( vAge / MAX_AGE, 0.0, BURST_DURATION ) / BURST_DURATION) * vBurstVel * DeltaTime;
	oBurstVel = vBurstVel;
	oAge = vAge + DeltaTime;
	oBucketIndex = vBucketIndex;

	if( oAge >= MAX_AGE )
	{
		uint seed = uint( Time * 1000.0 ) + uint( gl_VertexID );
		if( randhash( seed++, 1.0 ) < SPAWN_CHANCES[ vBucketIndex ] )
		{
			float xPhase = randhash( seed++, 1.0 );
			float yPhase = randhash( seed++, 1.0 );
			oPosition.x = SPAWN_AREAS[ vBucketIndex ].y * xPhase + SPAWN_AREAS[ vBucketIndex ].x;
			oPosition.y = SPAWN_AREAS[ vBucketIndex ].w * yPhase + SPAWN_AREAS[ vBucketIndex ].z;
			oVelocity = vec2( 0.0, 1.0 );
			oBurstVel = vec2( randhash( seed++, 0.2 ) - 0.1, BURST_INTENSITY / MAX_AGE );
			oAge = 0.0;//MAX_AGE * ENERGY_MAX_AGE_FACTOR * (1.0 - SPAWN_CHANCES[ vBucketIndex ] );
		}
	}
}
)";
