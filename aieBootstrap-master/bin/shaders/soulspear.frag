// FRAGMENT SHADER
#version 410

in vec2 vUV;
in vec3 vNormal;
in vec3 vPosition;

out vec4 FragColour;

uniform vec3 LightDir;
uniform vec3 LightColour;
uniform vec3 CameraPos;
uniform float SpecPow;

void main()
{
    // Diffuse
	float d = max(0, dot(normalize(vNormal.xyz), LightDir));
	
	vec3 E = normalize(CameraPos - vPosition.xyz);
	vec3 R = reflect( -LightDir, vNormal.xyz);
	float s = max(0, dot(E,R));
	s = pow(s, SpecPow);
	
	FragColour = vec4(LightColour * d + LightColour * s, 1);
}
