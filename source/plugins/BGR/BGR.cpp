#include "BGR.h"

static PluginInstance p = Effect::createPlugin< BGR >( {
	"FL14",            // plugin unique ID
	"BGR"// Plugin name
} );

static const std::string fshader = R"(
void main()
{
	vec4 color = texture(inputTexture, i_uv);
	color.rgb = color.bgr;
	fragColor = color;
}
)";

BGR::BGR()
{
	setFragmentShader( fshader );
}

BGR::~BGR()
{
}
