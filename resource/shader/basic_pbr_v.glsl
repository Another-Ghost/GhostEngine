#version 450 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_coords;
layout (location = 2) in vec3 normal;


out vec2 TexCoords;

uniform mat4 model;

struct CameraInfo
{
    vec4 position;
    mat4 view;
    mat4 projection;
};

layout(std140, binding = 0) uniform Camera
{
    CameraInfo camera;
};

void main()
{
    TexCoords = tex_coords;    
    gl_Position = camera.projection * camera.view * model * vec4(pos, 1.0);
}