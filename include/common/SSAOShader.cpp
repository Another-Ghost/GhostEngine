#include "SSAOShader.h"

TextureUnit SSAOShader::noise_texture_tu{ "noise_texture", 7 };
SSAOShader::SSAOShader(const string& vertex_path /*= File::GetShaderPath("basic_v")*/, const string& fragment_path /*= File::GetShaderPath("ssao_f")*/, const string& geometry_path /*= ""*/):
	MVPShader(vertex_path, fragment_path, geometry_path)
{
	Initialize(false);
}

void SSAOShader::Initialize(bool b_reload)
{
	UseTextureUnit(TextureUnit::g_position);
	UseTextureUnit(TextureUnit::g_normal);
	UseTextureUnit(noise_texture_tu);

	if (b_reload)
	{
		MVPShader::Initialize(b_reload);
	}
}



