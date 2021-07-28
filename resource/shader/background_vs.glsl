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
	vec4 clip_pos = projection * rot_view * vec4(WorldPos, 1.0);	//反正经过这一变换，在clip space里顶点position值只旋转，大小没有随透视而缩放（还是1），所以输出的时候能铺满视口大小

//	vec4 clip_pos = rot_view * vec4(WorldPos, 1.0);
//	clip_pos.w = 1;
	//https://blog.csdn.net/iispring/article/details/27970937
	gl_Position = clip_pos.xyww;	// 在由clip space变换到NDC时，xyz都需要除以w，这里的trick是，因为z = w, 所以变换后的z永远等于1, 即NDC中的深度最小值（范围为（-1, 1)，左手坐标系）
}