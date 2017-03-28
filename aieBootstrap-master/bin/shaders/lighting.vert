#version 410
layout(location=0) in vec4 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uv;

uniform mat4 PVM;

out vec3 vNormal;
out vec2 vUV;

void main()
{
	gl_Position = PVM * position;

	vNormal = normal;
	
	vUV = uv;
	vUV.y = -vUV.y;
}



