#pragma once
#include "common\PBRShader.h"

class WFTestShader : public PBRShader
{
public:
	WFTestShader(const string& vertex_path = File::GetShaderPath("pbr_vs"), const string& fragment_path = File::GetShaderPath("white_furnace_test_pbr_f"), const string& geometry_path = "");
	
	void SetRoughness(float roughness_);

	void BindSSDirectionalAlbedoLUT(unsigned int tex_id);

private:
	float roughness;
};

