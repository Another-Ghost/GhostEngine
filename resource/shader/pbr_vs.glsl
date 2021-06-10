#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_coords;
layout (location = 2) in vec3 normal;


out vec2 Tex_Coords;
out vec3 World_Pos;
out vec3 Normal;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    Tex_Coords = tex_coords;
    World_Pos = vec3(model * vec4(pos, 1.0));
    Normal = mat3(model) * normal;   

    gl_Position =  projection * view * vec4(World_Pos, 1.0);
}


