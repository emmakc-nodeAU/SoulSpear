// VERTEX SHADER
#version 410

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Normal;

out vec4 vNormal;
out vec4 vPosition;	// Specular

uniform mat4 projectionViewWorldMatrix;
uniform mat4 modelMatrix;

void main()
{
    vNormal = Normal;
	vPosition = Position;
	gl_Position = projectionViewWorldMatrix * Position;
	
	// mat4 PVM = projectionViewWorldMatrix * modelMatrix;
}