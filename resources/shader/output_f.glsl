#version 450 core

out vec4 OUT_FragColor;

in vec2 TexCoords;

uniform sampler2D g_color;
uniform sampler2D ssao;

void main()
{
	vec3 color = texture(g_color, TexCoords).rgb;
	//float occlusion = texture(ssao, TexCoords).r;
	vec3 occlusion = texture(ssao, TexCoords).rgb;

	OUT_FragColor = vec4((color * occlusion.r), 1.f);
}