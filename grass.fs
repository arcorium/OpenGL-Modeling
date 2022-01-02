#version 460 core

in vec3 sPos;
in vec2 sTexCoord;

uniform sampler2D tex;

out vec4 FragColor;

void main()
{
	vec4 temp = texture(tex, sTexCoord);
//	if (temp.a < 0.1f)
//		discard;	// stop processing current fragment
	FragColor = texture(tex, sTexCoord);
}