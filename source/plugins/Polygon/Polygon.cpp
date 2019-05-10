#include "Polygon.h"

static PluginInstance p = Source::createPlugin< PolygonRepeat >( {
	"FL05",  // plugin unique ID
	"Polygon"// Plugin name
} );

static const std::string fshader = R"(
#define PI 3.14159265359
#define TWO_PI 6.28318530718
#define pixel_width 50.*repeat*3./resolution.y
uniform float relativeTime;
uniform float relativeRotationTime;

float stroke(float d, float size) {
	return smoothstep(pixel_width,0.0,abs(d-size)-width/2.);
}

vec2 rotate(vec2 _uv, float _angle){
    _uv =  mat2(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle)) * _uv;
    return _uv;
}

float polygonSDF(vec2 _uv) {
	// Angle and radius from the current pixel
	float a = atan(_uv.x,_uv.y)+PI;
	float r = TWO_PI/float(floor(sides));

	return cos(floor(.5+a/r)*r-a)*length(_uv);
}

float smoothmodulo(float a) {
	return abs( mod(a, 2.) - 1.);
}

void main()
{
    vec2 U = i_uv*2.-1.;
	U.x *= resolution.x/resolution.y;
    U = rotate(U,relativeRotationTime);
    U *= repeat*50.;
    float c = stroke(smoothmodulo(polygonSDF(U)+relativeTime*10.),1.);
    fragColor = vec4(vec3(c),1.);                                       
}
)";

PolygonRepeat::PolygonRepeat()
{
	setFragmentShader( fshader );
	addParam( ParamRange::create( "repeat", 0.05f, { 0.0f, 1.0f } ) );
	addParam( ParamRange::create( "speed", 0.4f, { -.5f, .5f } ) );
	addParam( ParamRange::createInteger( "sides", 3, { -20, 200 } ) );
	addParam( ParamRange::create( "width", 0.5f, { 0.0f, 2.0f } ) );
	addParam( ParamRange::create( "rotation_speed", 0.5, { -2.0f, 2.0f } ) );
}

void PolygonRepeat::update()
{
	auto speedParam = std::dynamic_pointer_cast< ParamRange >( getParam( "speed" ) );
	float speed     = speedParam->getRealValue();
	relativeTime += deltaTime * speed;
	glUniform1f( shader.FindUniform( "relativeTime" ), relativeTime );
	auto speedRotationParam = std::dynamic_pointer_cast< ParamRange >( getParam( "rotation_speed" ) );
	float speedRotation     = speedRotationParam->getRealValue();
	relativeRotationTime += deltaTime * speedRotation;
	glUniform1f( shader.FindUniform( "relativeRotationTime" ), relativeRotationTime );
}

PolygonRepeat::~PolygonRepeat()
{
}