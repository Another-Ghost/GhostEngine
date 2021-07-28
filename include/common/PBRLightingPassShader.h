#pragma once
#include "MVPShader.h"

class PBRLightingPassShader : public MVPShader
{
public:
	PBRLightingPassShader(const string& vertex_path = File::GetShaderPath("basic_v"), const string& fragment_path = File::GetShaderPath("pbr_lighting_pass_f"), const string& geometry_path = "");

	void Initialize(bool b_reload) override;

	void Draw();
};

