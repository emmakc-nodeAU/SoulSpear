// VERTEX SHADER
#version 410

layout(location=0) in vec3 Position;
layout(location=1) in vec3 Normal;
layout(location=2) in vec2 UV;

out vec2 vUV;
out vec3 vNormal;
out vec3 vPosition;

uniform mat4 projectionViewWorldMatrix;
uniform mat4 modelMatrix;

void main()
{
    vNormal = Normal;
	vPosition = Position;
	vUV = UV;
	
	gl_Position = projectionViewWorldMatrix * modelMatrix * vec4(Position, 1);
	
	// mat4 PVM = projectionViewWorldMatrix * modelMatrix;
}