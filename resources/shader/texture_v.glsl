#version 450 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_coords;

out vec2 Tex_Coords;

void main()
{
    Tex_Coords = tex_coords;
	gl_Position = vec4(pos, 1.0);
}