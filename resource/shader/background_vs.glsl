#version 330 core
layout (location = 0) in vec3 pos;

uniform mat4 projection;
uniform mat4 view;

out vec3 WorldPos;
//out vec3 FragPos;

void main()
{
    WorldPos = pos;

	mat4 rot_view = mat4(mat3(view));//�Ƴ�λ��

	//FragPos = (rot_view * vec4(WorldPos, 1.0)).xyz;
	vec4 clip_pos = projection * rot_view * vec4(WorldPos, 1.0);	//����������һ�任����clip space�ﶥ��positionֵֻ��ת����Сû����͸�Ӷ����ţ�����1�������������ʱ����������Ļ

//	vec4 clip_pos = rot_view * vec4(WorldPos, 1.0);
//	clip_pos.w = 1;
	gl_Position = clip_pos.xyww;
}