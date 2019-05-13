#include "Gradient.h"

static PluginInstance p = Source::createPlugin< Gradient >( {
	"FL07",          // plugin unique ID
	"Gradient FFGL"  // Plugin name
} );

static const std::string fshader = R"(
void main()
{
	fragColor = mix( RGBLeft, RGBRight, i_uv.x );
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