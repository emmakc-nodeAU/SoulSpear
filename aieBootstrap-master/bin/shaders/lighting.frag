#version 410

in vec3 vNormal;
in vec3 colour;
in vec2 vUV;

uniform sampler2D myTextureSampler;
uniform vec4 LightAmbient = vec4(0.2f,0.2f,0.2f,1);
uniform vec4 LightDiffuse = vec4(0.5f, 0, 0, 1);

out vec4 fragColour;

void main()
{
	vec4 diffuse = texture(myTextureSampler, vUV);
	fragColour = diffuse;
	
	
	float d = max(0, dot(normalize(vNormal), vec3(0,1,0) ) );
	
	vec4 finalAmbient = LightAmbient * diffuse;
	vec4 finalDiffuse = diffuse * (d * LightDiffuse);
	fragColour = finalAmbient + finalDiffuse;
}  