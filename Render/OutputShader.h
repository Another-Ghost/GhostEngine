#pragma once
#include "Shader.h"
class OutputShader : public Shader
{
public:
	OutputShader(const string& vertex_path = File::GetShaderPath("basic_v"), const string& fragment_path = File::GetShaderPath("output_f"), const string& geometry_path = "");

	void Initialize(bool b_reload) override;


};

