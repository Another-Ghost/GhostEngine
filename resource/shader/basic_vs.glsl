#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_coords;
layout (location = 2) in vec3 normal;

out vertex
{
    vec2 tex_coords;
    vec3 world_pos;
    vec3 normal;
} OUT;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    OUT.tex_coords = tex_coords;
    OUT.world_pos = vec3(model * vec4(pos, 1.0));
    OUT.normal = mat3(model) * normal;   

    gl_Position =  projection * view * vec4(OUT.world_pos, 1.0);
}


