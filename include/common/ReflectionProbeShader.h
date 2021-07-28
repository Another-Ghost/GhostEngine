#pragma once
#include "MVPShader.h"
class ReflectionProbeShader : public MVPShader
{
	ReflectionProbeShader(const string& vertex_path = File::GetShaderPath("cubemap_vs"), const string& fragment_path = File::GetShaderPath("reflection_probe_f"), const string& geometry_path = "");
};

