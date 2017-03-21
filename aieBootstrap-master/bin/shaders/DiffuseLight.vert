#version 410

in vec4 vPosition;
in vec4 vColour;
in vec2 vUv;
in vec3 vNormal;

out vec4 fColour;
out vec2 fuv;
out vec3 fNormal;
out vec3 fPos;

uniform mat4 projectionView;
uniform mat4 model;

void main ()
{
	fuv = vUv;
	fColour = vColour;
	fNormal = vNormal;
	fPos = (model * vPosition).xyz;
	gl_Position = projectionView * model * vPosition;
}