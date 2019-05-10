#include "BarrelBlur.h"

static PluginInstance p = Effect::createPlugin< BarrelBlur >( {
	"FL08",      // plugin unique ID
	"Barrel Blur"// Plugin name
} );

static const std::string fshader = R"(
vec3 spectrum_offset_rgb( float t )
{
    float t0 = 3.0 * t - 1.5;
	vec3 ret = clamp( vec3( -t0, 1.0-abs(t0), t0), 0.0, 1.0);
    
    return ret;
}

vec2 barrelDistortion( vec2 p, vec2 amt )
{
    p = 2.0 * p - 1.0;
    const float maxBarrelPower = 5.0;
    float theta  = atan(p.y, p.x);
    vec2 radius = vec2( length(p) );
    radius = pow(radius, 1.0 + maxBarrelPower * amt);
    p.x = radius.x * cos(theta);
    p.y = radius.y * sin(theta);

    return p * 0.5 + 0.5;
}

vec2 brownConradyDistortion(vec2 uv, float dist)
{
    uv = uv * 2.0 - 1.0;
    // positive values of K1 give barrel distortion, negative give pincushion
    float barrelDistortion1 = 0.1 * dist; // K1 in text books
    float barrelDistortion2 = -0.025 * dist; // K2 in text books

    float r2 = dot(uv,uv);
    uv *= 1.0 + barrelDistortion1 * r2 + barrelDistortion2 * r2 * r2;
    //uv *= 1.0 + barrelDistortion1 * r2;
    
    // tangential distortion (due to off center lens elements)
    // is not modeled in this function, but if it was, the terms would go here
    return uv * 0.5 + 0.5;
}

vec2 distort( vec2 uv, float t, vec2 min_distort, vec2 max_distort )
{
    vec2 dist = mix( min_distort, max_distort, t );
	if(conrady) {
		return brownConradyDistortion( uv, 75.0 * dist.x );
	}
    return barrelDistortion( uv, 1.75* dist ); //distortion at center
}

void main()
{    
    const float MAX_DIST_PX = 50.0;
    float max_distort_px = MAX_DIST_PX * barrelAmount;
	vec2 max_distort = vec2(max_distort_px) / resolution.xy;
    vec2 min_distort = 0.5 * max_distort;
    
    vec2 oversiz = distort( vec2(1.0), 1.0, min_distort, max_distort );
    vec2 m_uv = mix(i_uv, map( i_uv, 1.0-oversiz, oversiz ),zoomAmount) ;
    
    const int num_iter = 7;
    const float stepsiz = 1.0 / (float(num_iter)-1.0);
    float rnd = random();
    float t = rnd*stepsiz;
    
    vec3 sumcol = vec3(0.0);
	vec3 sumw = vec3(0.0);
	for ( int i=0; i<num_iter; ++i )
	{
		vec3 w = spectrum_offset_rgb( t );
		sumw += w;
        vec2 uvd = distort(m_uv, t, min_distort, max_distort); //TODO: move out of loop
		sumcol += w * texture( inputTexture, uvd ).rgb;
        t += stepsiz;
	}
    sumcol.rgb /= sumw;
    
    vec3 outcol = sumcol.rgb;
    outcol =  outcol;
    outcol += rnd/255.0;
    
	fragColor = vec4(outcol, 1.0);
}
)";

BarrelBlur::BarrelBlur()
{
	include( { shader::snippet_id::map, shader::snippet_id::random } );
	setFragmentShader( fshader );
	addParam( ParamRange::create( "barrelAmount", 0.5f, { 0.0, 2.0 } ) );
	addParam( ParamRange::create( "zoomAmount", 0.5f, { 0.0, 1.0 } ) );
	addParam( ParamBool::create( "conrady" ) );
}

BarrelBlur::~BarrelBlur()
{
}