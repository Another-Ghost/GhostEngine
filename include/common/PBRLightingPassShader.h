#pragma once
#include "PBRShader.h"

class PBRLightingPassShader : public PBRShader
{
public:
	PBRLightingPassShader(const string& vertex_path = File::GetShaderPath("pbr_vs"), const string& fragment_path = File::GetShaderPath("pbr_lighting_pass_f"), const string& geometry_path = "");

	void Initialize(bool b_reload) override;

};

