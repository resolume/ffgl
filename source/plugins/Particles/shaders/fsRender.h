#pragma once

static const char fsRender[] = R"(#version 410 core
uniform sampler2D ParticleTexture;

in vec2 uv;
in vec4 color;

out vec4 fragColor;

void main()
{
	fragColor = color * texture( ParticleTexture, uv ).rrra;
}

)";