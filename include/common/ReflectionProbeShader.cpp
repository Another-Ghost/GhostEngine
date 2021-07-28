#include "ReflectionProbeShader.h"

ReflectionProbeShader::ReflectionProbeShader(const string& vertex_path /*= File::GetShaderPath("cubemap_vs")*/, const string& fragment_path /*= File::GetShaderPath("reflection_probe_f")*/, const string& geometry_path /*= ""*/):
	MVPShader(vertex_path, fragment_path, geometry_path)
{

}
