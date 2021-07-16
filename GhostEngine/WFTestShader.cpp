#include "WFTestShader.h"

WFTestShader::WFTestShader(const string& vertex_path, const string& fragment_path, const string& geometry_path)
	:PBRShader(vertex_path, fragment_path, geometry_path)
{

}

void WFTestShader::SetRoughness(float roughness_)
{
	Use();
	roughness = roughness_;
	SetFloat("roughness", roughness);
}

void WFTestShader::BindSSDirectionalAlbedoLUT(unsigned int tex_id)
{
	glActiveTexture(GL_TEXTURE0 + texture_name_unit_map["ssda_lut"]);
	glBindTexture(GL_TEXTURE_2D, tex_id);
}
