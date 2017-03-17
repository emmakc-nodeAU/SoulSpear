// FRAGMENT SHADER - USE SHADOW
#version 410

in vec4 vNormal;
in vec4 vPosition;

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

/*
Definitions:
vNormal = Bunny vertex normal
lightColour = replaces hardcoded vec3(0,1,0)
lightDir = holds a vec3 (vec3(0,1,0)
- Points to Direction of light origin
- Used by ShadowMap
*/