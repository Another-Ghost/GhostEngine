#pragma once
#include "MVPShader.h"
class BRDFLUTShader : public MVPShader
{
public:
	BRDFLUTShader(const string& vertex_path = File::GetShaderPath("brdf_vs"), const string& fragment_path = File::GetShaderPath("brdf_fs"), const string& geometry_path = "");

	void RenderBRDFLUT(const Texture* brdf_lut);
};

