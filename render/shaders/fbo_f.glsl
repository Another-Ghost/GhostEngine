#version 450 core

out vec4 OUT_FragColor;

in vec2 TexCoords;

uniform sampler2D texture_0;

void main()
{
	vec4 color = texture(texture_0, TexCoords);

	OUT_FragColor = color;
}