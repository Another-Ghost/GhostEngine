#pragma once
#include "Shader.h"

class SSAOBlurShader : public Shader
{
public:
	SSAOBlurShader(const string& vertex_path = File::GetShaderPath("basic_v"), const string& fragment_path = File::GetShaderPath("ssao_blur_f"), const string& geometry_path = "");

	void Initialize(bool b_reload) override;

};

