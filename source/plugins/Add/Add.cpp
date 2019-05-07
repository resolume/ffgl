#include "Add.h"

static PluginInstance p = Mixer::createPlugin< Add >( {
	"FL03",   // plugin unique ID
	"Add FFGL"// Plugin name
} );

static const std::string fshader = R"(
void main()
{
	//get the two fragments to mix
	vec4 colorDest = texture( textureDest, i_uv_dest );
	vec4 colorSrc  = texture( textureSrc, i_uv_src );

	//here we add the colorSrc r,g,b,a pixel value to the colorDest pixel value according to the mixVal value
	vec4 mix = colorDest + colorSrc * mixVal;

	//Here we use the built-in function min(val1,val2) to get the minimum between val1 and val2 and always keep output pixel value between 0.0 and 1.0
	fragColor = min( mix, 1.0 );
}
)";

Add::Add()
{
	setFragmentShader( fshader );
}

Add::~Add()
{
}
