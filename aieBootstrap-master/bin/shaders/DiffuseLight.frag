#version 410
in vec4 fColour;
in vec2 fuv;
in vec3 fNormal;
in vec3 fPos;

out vec4 frag_colour;

uniform sampler2D texture;
uniform vec3 lightPos;
uniform vec3 lightColour;
uniform float lightAmbientStrength;

void main ()
{
	vec3 normal = normalize(fNormal);
	vec3 lightDir = normalize(fPos - lightPos);

	float diff = max(dot(normal, -lightDir), 0.0);
	vec3 diffuse = lightColour * diff;

	vec3 ambient = lightAmbientStrength * lightColour;
	
	frag_colour = vec4((ambient + diffuse), 1.0f) * texture2D(texture, fuv) * fColour;
}
