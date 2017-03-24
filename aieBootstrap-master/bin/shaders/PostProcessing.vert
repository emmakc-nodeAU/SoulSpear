#version 410
layout(location=0) in vec4 position;
layout(location=1) in vec2 TexCoord;

out vec2 vTexCoord;

uniform mat4 projectionViewMatrix;

void main()
{
	vTexCoord = TexCoord;
	gl_Position = projectionViewMatrix * position;
}


