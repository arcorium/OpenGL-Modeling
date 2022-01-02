#version 460 core

in vec3 sFragPos;

out vec4 FragColor;

uniform vec3 Color;


void main()
{
	vec3 result = Color * sFragPos;
	FragColor = vec4(result, 1.0f);
}