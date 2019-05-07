#include "MovingGradient.h"

static PluginInstance p = Source::createPlugin< MovingGradient >( {
	"FL07",          // plugin unique ID
	"Moving Gradient"// Plugin name
} );

static const std::string fshader = R"(
void main()
{
	vec2 uv = i_uv;
	uv.x *= resolution.x/resolution.y;
    
    // Time varying pixel color
    vec3 col = 0.5 + 0.5*cos(time+uv.xyx+vec3(0,2,4));

    vec2 bl = step(vec2(amount),uv);       // bottom-left
    vec2 tr = step(vec2(amount),vec2(1.*resolution.x/resolution.y,1.)-uv);   // top-right
    vec3 color = vec3(bl.x * bl.y * tr.x * tr.y);

    // Output to screen
    fragColor = vec4(col - color,1.0);
}
)";

MovingGradient::MovingGradient()
{
	setFragmentShader( fshader );
	addParam( ParamRange::create( "amount", 0.5f, { 0., 0.5 } ) );
}

MovingGradient::~MovingGradient()
{
}