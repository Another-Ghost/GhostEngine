#include "LightingPassShader.h"

LightingPassShader::LightingPassShader(const string& vertex_path /*= File::GetShaderPath("pbr_vs")*/, const string& fragment_path /*= File::GetShaderPath("pbr_lighting_pass_f")*/, const string& geometry_path /*= ""*/)
	:PBRShader(vertex_path, fragment_path, geometry_path)
{

}
