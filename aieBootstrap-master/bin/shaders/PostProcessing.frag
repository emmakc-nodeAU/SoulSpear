#version 410

in vec2 vTexCoord;
out vec4 fragColour;

uniform sampler2D diffuse;

void main()
{
	fragColour = texture(diffuse, vTexCoord);
}