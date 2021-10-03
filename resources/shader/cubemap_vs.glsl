#version 330 core
layout (location = 0) in vec3 pos;

out vec3 World_Pos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    World_Pos = pos;
    gl_Position =  projection * view * vec4(World_Pos, 1.0);
}