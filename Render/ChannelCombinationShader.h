#pragma once
#include "Shader.h"
class ChannelCombinationShader : public Shader
{
public:
	ChannelCombinationShader(const string& vertex_path = File::GetShaderPath("texture_v"), const string& fragment_path = File::GetShaderPath("channel_combination_f"), const string& geometry_path = "");

	void RenderTexture(const PlaneTexture* out_tex, const PlaneTexture* tex1, const PlaneTexture* tex2);	//? 改成可以控制输入到哪个channel, 输入tex数量（重构）
};

