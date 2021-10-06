#version 450 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_coords;
layout (location = 2) in vec3 normal;


out vec2 TexCoords;
out vec3 WorldPos;
out vec3 WorldNormal;
out vec3 ViewPos;
out vec3 ViewNormal;


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

//uniform mat4 projection;
//uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = tex_coords;
    WorldPos = vec3(model * vec4(pos, 1.0));
    WorldNormal = mat3(model) * normal;   

    //WorldPos = FragPos;
    ViewPos = (camera.view * vec4(WorldPos, 1.0)).xyz;
    //https://zhuanlan.zhihu.com/p/72734738
    //mat3 normal_matrix =  transpose(inverse(mat3(camera.view * model)));   //inverse(model)改成uniform传进去（CameraInfo里加一个）/? 改成求3维的逆而不是4维只是为了减小计算量
    mat3 normal_matrix =  mat3(camera.view * model);   //inverse(model)改成uniform传进去（CameraInfo里加一个）/? 改成求3维的逆而不是4维只是为了减小计算量
    ViewNormal = normal_matrix * normal;

    gl_Position =  camera.projection * camera.view * vec4(WorldPos, 1.0);

     
}


