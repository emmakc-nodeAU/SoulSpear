#version 410
layout(location=0) in vec4 position;
layout(location=1) in vec4 colour;

out vec4 vColour;

uniform mat4 projectionViewMatrix;
uniform mat4 modelMatrix;

void main()
{
	mat4 PVM = projectionViewMatrix * modelMatrix;
	gl_Position = PVM * position;
	vColour = colour;
}


