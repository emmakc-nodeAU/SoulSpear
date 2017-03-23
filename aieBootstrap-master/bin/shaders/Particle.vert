#version 410

layout(location=0)in vec4 vPosition;
layout(location=1)in vec4 vColour;
layout(location=2)in vec2 vUv;
layout(location=3)in vec3 vNormal;
layout(location=4)in vec2 vtexCoord;

out vec4 fColour;
out vec2 fuv;
out vec3 fNormal;
out vec3 fPos;
out vec2 fTexCoord;

uniform mat4 projectionView;
uniform mat4 model;

void main ()
{
	fuv = vUv;
	fColour = vColour;
	fNormal = vNormal;
	fPos = (model * vPosition).xyz;
	gl_Position = projectionView * model * vPosition;
	fTexCoord = vtexCoord;
}