#pragma once
#include "MVPShader.h"
class PBRGeometryPassShader : public MVPShader
{
	PBRGeometryPassShader(const string& vertex_path = File::GetShaderPath("pbr_geometry_pass_v"), const string& fragment_path = File::GetShaderPath("pbr_gometry_pass_f"), const string& geometry_path = "");

	void Initialize(bool b_reload) override;
};

