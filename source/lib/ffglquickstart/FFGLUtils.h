#pragma once
#include <algorithm>
#include <map>
#include <set>

namespace ffglqs
{
namespace shader
{
enum snippet_id
{
	random,
	map,
	simplex
};

static const std::map< snippet_id, std::set< snippet_id > > dependencies = {
	{ simplex, {} }
};

static const std::map< snippet_id, std::string > snippets = {

	// Hardware indepedant random [0;1]
	{ snippet_id::random, R"(
const uint random_k = 1103515245U;  // GLIB C

float random( uvec2 x )
{
	uvec2 q = random_k * ( (x>>1U) ^ (x.yx   ) );
	uint  n = random_k * ( (q.x  ) ^ (q.y>>3U) );
	return float(n) * (1.0/float(0xffffffffU));
}

vec3 random3( uvec3 x )
{
	x = ((x>>8U)^x.yzx)*random_k;
	x = ((x>>8U)^x.yzx)*random_k;
	x = ((x>>8U)^x.yzx)*random_k;

	return vec3(x)*(1.0/float(0xffffffffU));
}

float random()
{
	uvec2 p = uvec2(gl_FragCoord) + uint(resolution.x) * uint(resolution.y) * uint(frame);
	return random(p);
}
)" },

	{ snippet_id::map, R"(
float map( float t, float a, float b )
{
	return clamp( (t - a) / (b - a), 0.0, 1.0 );
}

vec2 map( vec2 t, vec2 a, vec2 b )
{
	return clamp( (t - a) / (b - a), 0.0, 1.0 );
}

float map( float value, float low, float high, float newLow, float newHigh )
{
	float res = newLow + (value - low) * (newHigh - newLow) / (high - low);
	return clamp(res, newLow, newHigh);
}

vec2 map(vec2 value, vec2 low, vec2 high, vec2 newLow, vec2 newHigh) {
		vec2 res = newLow + (value - low) * (newHigh - newLow) / (high - low);
		return clamp(res, newLow, newHigh);
}
)" },

	{ snippet_id::simplex, R"(
vec3 random3( vec3 c )
{
	float j = 4096.0*sin(dot(c,vec3(17.0, 59.4, 15.0)));
	vec3 r;
	r.z = fract(512.0*j);
	j *= .125;
	r.x = fract(512.0*j);
	j *= .125;
	r.y = fract(512.0*j);
	return r;
}
const float F3 =  0.3333333;
const float G3 =  0.1666667;

float simplex3d(vec3 p)
{
	vec3 s = floor(p + dot(p, vec3(F3)));
	vec3 x = p - s + dot(s, vec3(G3));

	vec3 e = step(vec3(0.0), x - x.yzx);
	vec3 i1 = e*(1.0 - e.zxy);
	vec3 i2 = 1.0 - e.zxy*(1.0 - e);
	
	vec3 x1 = x - i1 + G3;
	vec3 x2 = x - i2 + 2.0*G3;
	vec3 x3 = x - 1.0 + 3.0*G3;

	vec4 w, d;

	w.x = dot(x, x);
	w.y = dot(x1, x1);
	w.z = dot(x2, x2);
	w.w = dot(x3, x3);

	w = max(0.6 - w, 0.0);

	d.x = dot(random3(s)-.5, x);
	d.y = dot(random3(s + i1)-.5, x1);
	d.z = dot(random3(s + i2)-.5, x2);
	d.w = dot(random3(s + 1.0)-.5, x3);

	w *= w;
	w *= w;
	d *= w;

	return dot(d, vec4(52.0));
}

const mat3 rot1 = mat3(-0.37, 0.36, 0.85,-0.14,-0.93, 0.34,0.92, 0.01,0.4);
const mat3 rot2 = mat3(-0.55,-0.39, 0.74, 0.33,-0.91,-0.24,0.77, 0.12,0.63);
const mat3 rot3 = mat3(-0.71, 0.52,-0.47,-0.08,-0.72,-0.68,-0.7,-0.45,0.56);

float fractal_noise(vec3 m, vec4 coef)
{
	return   coef.x*simplex3d(m*rot1)
			+coef.y*simplex3d(2.0*m*rot2)
			+coef.z*simplex3d(4.0*m*rot3)
			+coef.w*simplex3d(8.0*m);
}
)" }
};

}// namespace shader

}//End namespace ffglqs