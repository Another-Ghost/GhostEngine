#pragma once
#include "MVPShader.h"
class BasicShader : public MVPShader
{
public:
	BasicShader(const string& vertex_path = File::GetShaderPath("basic_pbr_v"), const string& fragment_path = File::GetShaderPath("basic_f"), const string& geometry_path = "");

	void BindMaterial(BasicMaterial* material);
};

