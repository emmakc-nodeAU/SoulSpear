// FRAGMENT SHADER
#version 410

in vec3 vNormal;
in vec2 vUV;
in vec3 vPosition;

out vec4 FragColour;

uniform sampler2D myTextureSampler;
uniform vec4 LightAmbient = vec4(1.0f, 1.0, 1.0f, 1);
uniform vec4 LightDiffuse = vec4(0.5f, 0, 0, 1);
uniform vec3 LightPosition = vec3(0, 1, 0);
//uniform vec3 LightDir;
//uniform vec3 LightColour;
//uniform vec3 CameraPos;
//uniform float SpecPow;

void main()
{
    // DIFFUSE
	vec4 diffuse = texture(myTextureSampler, vUV);
	FragColour = diffuse;

	// Cal direction
	vec3 lightDir = vPosition - LightPosition;
	lightDir = normalize(lightDir);
	
	float d = max(0, dot(normalize(vNormal.xyz), -lightDir ) );
	
	vec4 finalAmbient = LightAmbient * diffuse;
	vec4 finalDiffuse = diffuse * (d * LightDiffuse);

	FragColour = finalAmbient + finalDiffuse;

	//vec3 E = normalize(CameraPos - vPosition.xyz);
	//vec3 R = reflect( -LightDir, vNormal.xyz);
	//float s = max(0, dot(E,R));
	//s = pow(s, SpecPow);
	
}
