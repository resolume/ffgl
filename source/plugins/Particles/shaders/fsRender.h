#pragma once

static const char fsRender[] = R"(#version 330
#line 5
uniform sampler2D ParticleTexture;

in vec2 uv;
in vec4 color;

out vec4 fragColor;

void main()
{
	fragColor = color;// * vec4( vec3( 1.0 ), texture2D( ParticleTexture, uv ).a );
}

)";