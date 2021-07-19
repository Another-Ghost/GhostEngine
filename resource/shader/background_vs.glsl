#version 330 core
layout (location = 0) in vec3 pos;

uniform mat4 projection;
uniform mat4 view;

out vec3 WorldPos;
//out vec3 FragPos;

void main()
{
    WorldPos = pos;

	mat4 rot_view = mat4(mat3(view));//移除位移

	//FragPos = (rot_view * vec4(WorldPos, 1.0)).xyz;
	vec4 clip_pos = projection * rot_view * vec4(WorldPos, 1.0);	//反正经过这一变换，在clip space里顶点position值只旋转，大小没有随透视而缩放（还是1），所以输出的时候能铺满屏幕

//	vec4 clip_pos = rot_view * vec4(WorldPos, 1.0);
//	clip_pos.w = 1;
	gl_Position = clip_pos.xyww;
}