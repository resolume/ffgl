#include "Gradient.h"

static PluginInstance p = Source::createPlugin< Gradient >( {
	"FL07",          // plugin unique ID
	"Gradient FFGL"  // Plugin name
} );

static const std::string fshader = R"(
void main()
{
	fragColor = vec4( mix( RGBLeft, RGBRight, i_uv.x ), 1.0 );
}
)";

Gradient::Gradient()
{
	setFragmentShader( fshader );
	addHueColorParam( "RGBLeft" );
	addHueColorParam( "RGBRight" );
}

Gradient::~Gradient()
{
}