#pragma once
#include "PBRShader.h"

class LightingPassShader : public PBRShader
{
	LightingPassShader(const string& vertex_path = File::GetShaderPath("pbr_vs"), const string& fragment_path = File::GetShaderPath("pbr_lighting_pass_f"), const string& geometry_path = "");

};

