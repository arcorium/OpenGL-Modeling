#version 460 core

layout(location=0)in vec3 aPos;
layout(location=1)in vec2 aTexCoord;

out vec3 sPos;
out vec2 sTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 position = projection * view * model * vec4(aPos, 1.0f);
	gl_Position = position;

	sPos = vec3(model * vec4(aPos, 0.0f));
	sTexCoord = aTexCoord;
}