#include "StroboBeat.h"

static PluginInstance p = Effect::createPlugin< StroboBeat >( {
	"FL12",     // plugin unique ID
	"StroboBeat"// Plugin name
} );

static const std::string fshader = R"(
void main()
{
	vec4 tex = texture(inputTexture, i_uv);
	bool shouldBeOn = fract(phase * speed) < dutyCycle;
	shouldBeOn = reversephase ? !shouldBeOn : shouldBeOn;
	tex = mix(tex,vec4(0,0,0,1),blackframe);
	fragColor=mix(tex,color,shouldBeOn);
}
)";

StroboBeat::StroboBeat()
{
	addHueColorParam( "color" );
	addParam( dutyCycle = Param::create( "dutyCycle", .5f ) );
	addParam( speed = ParamRange::create( "speed", .5, { 1, 64 } ) );
	addParam( ParamBool::create( "blackframe" ) );
	addParam( ParamBool::create( "reversephase" ) );
	setFragmentShader( fshader );
}

void StroboBeat::update()
{
	unsigned long v = (unsigned long)speed->getRealValue();
	// get upper powe of two
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	float value = (float)v / 4.f;
	shader.Set( "speed",  value );
}

StroboBeat::~StroboBeat()
{
}