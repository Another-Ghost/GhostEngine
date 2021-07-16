#pragma once
#include "MVPShader.h"
class SSAOShader : public MVPShader
{
public:
	static TextureUnit noise_texture_tu;

	SSAOShader(const string& vertex_path = File::GetShaderPath("basic_v"), const string& fragment_path = File::GetShaderPath("ssao_f"), const string& geometry_path = "");

	void Initialize(bool b_reload) override;


	//void BindNoiseTexture(Texture* texture);
};

