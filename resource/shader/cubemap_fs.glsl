#version 330 core
out vec4 Frag_Color;
in vec3 World_Pos;

uniform vec3 color;

void main()
{		
    Frag_Color = vec4(color, 1.0);
}