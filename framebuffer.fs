#version 460 core

in vec2 sTexCoord;
in vec2 sPos;

out vec4 FragColor;

uniform sampler2D tex;

void main()
{
	FragColor = texture(tex, sTexCoord);
}