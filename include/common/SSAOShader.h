#pragma once
#include "MVPShader.h"
class SSAOShader : public MVPShader
{
	SSAOShader(const string& vertex_path = File::GetShaderPath("basic_v"), const string& fragment_path = File::GetShaderPath("ssao_f"), const string& geometry_path = "");
};

