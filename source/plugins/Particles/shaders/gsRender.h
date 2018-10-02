#pragma once

static const char gsRender[] = R"(#version 410 core
#line 5
layout( points ) in;
layout( triangle_strip, max_vertices = 4 ) out;

uniform ivec2 RenderSize;
uniform float ParticleSize;
uniform float VelocitySizeFactor;
uniform vec2 MAX_UV;

in vec2 particlePosition[ 1 ];
in vec4 particleColor[ 1 ];
in vec2 particleVelocity[ 1 ];

out vec2 uv;
out vec4 color;

void main()
{
	float invAspectRatio = RenderSize.y / RenderSize.x;
	vec2 particleSize = vec2( 2.0 ) / RenderSize * vec2( ParticleSize );

	vec2 particleUp = normalize( particleVelocity[ 0 ] );
	vec2 particleRight = vec2( -particleUp.y, particleUp.x );
	particleUp *= particleSize;
	particleUp *= (VelocitySizeFactor * length( particleVelocity[ 0 ] ));
	particleRight *= particleSize;

	vec2 corners[ 4 ];
	corners[ 0 ] = particlePosition[ 0 ] + particleUp - particleRight;
	corners[ 1 ] = particlePosition[ 0 ] + particleUp + particleRight;
	corners[ 2 ] = particlePosition[ 0 ] - particleUp - particleRight;
	corners[ 3 ] = particlePosition[ 0 ] - particleUp + particleRight;
	vec2 uvs[ 4 ];
	uvs[ 0 ] = vec2( 0.0, MAX_UV.y );
	uvs[ 1 ] = vec2( MAX_UV.x, MAX_UV.y );
	uvs[ 2 ] = vec2( 0.0, 0.0 );
	uvs[ 3 ] = vec2( MAX_UV.x, 0.0 );

	gl_Position = vec4( corners[ 0 ], 0.0, 1.0 );
	uv = uvs[ 0 ];
	color = particleColor[ 0 ];
	EmitVertex();

	gl_Position = vec4( corners[ 1 ], 0.0, 1.0 );
	uv = uvs[ 1 ];
	color = particleColor[ 0 ];
	EmitVertex();

	gl_Position = vec4( corners[ 2 ], 0.0, 1.0 );
	uv = uvs[ 2 ];
	color = particleColor[ 0 ];
	EmitVertex();

	gl_Position = vec4( corners[ 3 ], 0.0, 1.0 );
	uv = uvs[ 3 ];
	color = particleColor[ 0 ];
	EmitVertex();

	EndPrimitive();
}
)";
