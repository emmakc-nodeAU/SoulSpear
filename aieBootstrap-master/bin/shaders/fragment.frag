#version 410
in vec4 fColour;
in vec2 fuv;

out vec4 frag_colour;

uniform sampler2D texture;

void main ()
{
	frag_colour = texture2D(texture, fuv) * fColour;
}
