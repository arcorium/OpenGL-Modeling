#version 460 core

layout (location=0) in vec3 aPosition;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexCoord;
//layout (location=3) in vec3 aTangent;
//layout (location=4) in vec3 aBitangent;
//layout (location=5) in vec4 aBoneIds;
//layout (location=6) in vec4 aWeights;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform mat4 camera;

out vec2 TexCoord;
out vec3 Normal;
out vec3 ObjectPosition;

void main()
{
	vec4 position = projection * view * model * vec4(aPosition, 1.0f);
	gl_Position = position;

	TexCoord = aTexCoord;
	Normal = mat3(transpose(inverse(model))) * aNormal;
	ObjectPosition = vec3(model* vec4(aPosition, 1.0f));
}