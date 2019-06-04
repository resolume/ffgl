#include "Wobble.h"

static PluginInstance p = Effect::createPlugin< Wobble >( {
	"FL15",            // plugin unique ID
	"Wobble"// Plugin name
} );

static const std::string fshader = R"(
uniform float relativeTime;
void main()
{
	vec2 p = i_uv *2.-1.;
	float pos = relativeTime + length(p * size);
    vec2 offset = vec2(cos(pos), sin(pos));
	fragColor = texture( inputTexture, i_uv+ strength*offset );
}
)";

Wobble::Wobble()
{
	setFragmentShader( fshader );
	addParam( speed = ParamRange::create( "speed", 1., { 0.1, 5. } ) );
	addParam( size = ParamRange::create( "size", 1., { .7, 5. } ) );
	addParam( speed = ParamRange::create( "strength", 1., { 0.001, .1 } ) );
}

Wobble::~Wobble()
{
}

void Wobble::update()
{
	auto speedParam = std::dynamic_pointer_cast< ParamRange >( getParam( "speed" ) );
	float speed     = speedParam->getRealValue();
	relativeTime += deltaTime * speed;
	shader.Set( "relativeTime", relativeTime );
}
