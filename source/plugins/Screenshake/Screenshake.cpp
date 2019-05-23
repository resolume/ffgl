#include "Screenshake.h"

static PluginInstance p = Effect::createPlugin< Screenshake >( {
	"FL10",      // plugin unique ID
	"Screenshake"// Plugin name
} );

static const std::string fshader = R"(
uniform float relativeTime;
uniform vec2 amount;
void main()
{
   	vec3 p3 = vec3(0,0, relativeTime)*8.0+8.0;
    vec2 noise = vec2(simplex3d(p3),simplex3d(p3+10.));
	if (random) noise = vec2(random3(p3).x,random3(p3+1000.).x)*2.-1.;
	fragColor = vec4( texture( inputTexture, i_uv+noise*amount*0.1  ).rgb, 1.0);
}
)";

Screenshake::Screenshake()
{
	include( shader::snippet_id::simplex );
	setFragmentShader( fshader );
	addParam( amountX = ParamRange::create( "amountX", 0.1f, { 0.0, 1.0 } ) );
	addParam( amountY = ParamRange::create( "amountY", 0.1f, { 0.0, 1.0 } ) );
	addParam( ParamRange::create( "speed", 0.3f, { 0.0, 3. } ) );
	addParam( ParamBool::create( "random" ) );
}

Screenshake::~Screenshake()
{
}

void Screenshake::update()
{
	auto speedParam = std::dynamic_pointer_cast< ParamRange >( getParam( "speed" ) );
	float speed     = speedParam->getRealValue();
	relativeTime += deltaTime * speed;
	shader.Set( "relativeTime", relativeTime );
	shader.Set( "amount", amountX->getRealValue(), amountY->getRealValue());
}