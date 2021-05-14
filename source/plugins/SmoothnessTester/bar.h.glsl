#pragma once

namespace bar {

static const char vertex[] = R"(#version 410 core


// ----- vertex shader start -----
layout( location = 0 ) in vec4 vPosition;

uniform float Time;
uniform float Width;

void main()
{
	float x = vPosition.x;
	float y = vPosition.y * Width;
	float yOffset = mix(sin(Time), sin(Time * 0.75), x * 0.5 + 0.5);
	gl_Position = vec4(x, y + yOffset, vPosition.z, vPosition.w);
}
// ------ vertex shader end ------


)";
static const char fragment[] = R"(#version 410 core


// ----- fragment shader start ----
out vec4 fragColor;

void main()
{
	fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
// ------ fragment shader end -----


)";

}
