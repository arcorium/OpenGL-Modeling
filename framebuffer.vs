#version 460 core

layout(location=0) in vec2 aPos;
layout(location=1) in vec2 aTexCoord;

out vec2 sTexCoord;
out vec2 sPos;

void main()
{
	gl_Position = vec4(aPos, 0.0f, 1.0f);
	sTexCoord = aTexCoord;
	sPos = aPos;
}