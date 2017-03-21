#version 410

in vec4 vPosition;
in vec4 vColour;
in vec2 vUv;

out vec4 fColour;
out vec2 fuv;

uniform mat4 projectionView;

void main ()
{
	fuv = vUv;
	fColour = vColour;
	gl_Position = projectionView * vPosition;
}