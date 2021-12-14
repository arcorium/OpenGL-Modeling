#version 460 core

out vec4 FragColor;

uniform vec3 Colour;

void main()
{
	FragColor = vec4(Colour, 1.0f);
}