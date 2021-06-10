#version 330 core
layout (location = 0) in vec3 pos;

uniform mat4 projection;
uniform mat4 view;

out vec3 World_Pos;

void main()
{
    World_Pos = pos;

	mat4 rot_view = mat4(mat3(view));
	vec4 clip_pos = projection * rot_view * vec4(World_Pos, 1.0);

	gl_Position = clip_pos.xyww;
}