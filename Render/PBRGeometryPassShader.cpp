#include "PBRGeometryPassShader.h"

PBRGeometryPassShader::PBRGeometryPassShader(const string& vertex_path /*= File::GetShaderPath("pbr_geometry_pass_v")*/, const string& fragment_path /*= File::GetShaderPath("pbr_gometry_pass_f")*/, const string& geometry_path /*= ""*/):
	MVPShader(vertex_path, fragment_path, geometry_path)
{
	Initialize(false);
}

void PBRGeometryPassShader::Initialize(bool b_reload)
{
	UseTextureUnit(TextureUnit::basecolor_map);
	UseTextureUnit(TextureUnit::normal_map);
	UseTextureUnit(TextureUnit::ao_map);
	UseTextureUnit(TextureUnit::metalness_roughness_map);
	UseTextureUnit(TextureUnit::emissive_map);


	if (b_reload)
	{
		MVPShader::Initialize(b_reload);
	}
}

