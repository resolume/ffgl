#pragma once

static const char fsDebugRender[] = R"(#version 410 core
#line 5

in vec4 color;

out vec4 fragColor;

void main()
{
	fragColor = color;
}

)";