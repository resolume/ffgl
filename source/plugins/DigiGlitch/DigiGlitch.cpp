//
//  DigiGlitch.cpp
//  FFGLPlugins
//
//  Created by Joris on 19/02/2019.
//

#include "DigiGlitch.hpp"

static CFFGLPluginInfo PluginInfo ( PluginFactory<DigiGlitch>,
                                    "RM02",
                                    "DigiGlitch",
                                    2, 1,
                                    1, 0,
                                    FF_EFFECT,
                                    "Glitchimus Maximums",
                                    "Resolume FFGL Example"
                                   );

static const char vertexShader[] = R"(#version 410 core
layout ( location = 0 ) in vec4 Position;
layout ( location = 1 ) in vec2 vUV;

uniform vec2 MaxUV;

out vec2 uv;

void main()
{
    gl_Position = Position;
    uv = vUV * MaxUV;
}
)";

static const char fragmentShader[] = R"(#version 410 core
uniform sampler2D Texture1;
uniform sampler2D Texture2;

in vec2 uv;

out vec4 color;

void main()
{
    color = texture( Texture1, uv );
}
)";



                                   



DigiGlitch::DigiGlitch() { 
    <#code#>;
}

DigiGlitch::~DigiGlitch() noexcept { 
    <#code#>;
}

FFResult DigiGlitch::InitGL(const FFGLViewportStruct *vp) { 
    <#code#>;
}

FFResult DigiGlitch::DeInitGL() { 
    <#code#>;
}

FFResult DigiGlitch::ProcessOpenGL(ProcessOpenGLStruct *pOpenGLData) { 
    <#code#>;
}

FFResult DigiGlitch::SetFloatParameter(unsigned int index, float value) { 
    <#code#>;
}

float DigiGlitch::GetFloatParameter(unsigned int index) { 
    <#code#>;
}
