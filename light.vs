#version 460 core

layout (location=0) in vec3 aPosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 sFragPos;

void main()
{
	vec4 position = projection * view * model * vec4(aPosition, 1.0f);
	gl_Position = position;

	sFragPos = vec3(model * vec4(aPosition, 1.0f));
}