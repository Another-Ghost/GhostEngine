#pragma once
#include "Shader.h"
class ChannelCombinationShader : public Shader
{
public:
	ChannelCombinationShader(const string& vertex_path = File::GetShaderPath("texture_v"), const string& fragment_path = File::GetShaderPath("channel_combination_f"), const string& geometry_path = "");

	void RenderTexture(const Texture* out_tex, const Texture* tex_1, const Texture* tex_2);	//? �ĳɿ��Կ������뵽�ĸ�channel, ����tex�������ع���
};

