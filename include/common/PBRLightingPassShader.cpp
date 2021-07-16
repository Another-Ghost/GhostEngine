#include "PBRLightingPassShader.h"

PBRLightingPassShader::PBRLightingPassShader(const string& vertex_path /*= File::GetShaderPath("pbr_vs")*/, const string& fragment_path /*= File::GetShaderPath("pbr_lighting_pass_f")*/, const string& geometry_path /*= ""*/)
	:PBRShader(vertex_path, fragment_path, geometry_path)
{
	Initialize(false);
}

void PBRLightingPassShader::Initialize(bool b_reload)
{
	UseTextureUnit(TextureUnit::g_position);
	UseTextureUnit(TextureUnit::g_normal);
	UseTextureUnit(TextureUnit::g_color);
	UseTextureUnit(TextureUnit::ssao);

	if (b_reload)
	{
		PBRShader::Initialize(false);
	}
}

