#version 450 core

out vec3 OUT_Frag_Color;

in vec2 Tex_Coords;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
//	vec2 combined_color; 
//	combined_color.r = texture(map_1, Tex_Coords).r;
//	combined_color.g = texture(map_2, Tex_Coords).r;
	OUT_Frag_Color =  vec3(0, texture(tex1, Tex_Coords).r, texture(tex2, Tex_Coords).r);
}