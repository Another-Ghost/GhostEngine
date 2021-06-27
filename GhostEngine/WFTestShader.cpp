#include "WFTestShader.h"

WFTestShader::WFTestShader()
	:PBRShader(File::GetShaderPath("pbr_vs"), File::GetShaderPath("white_furnace_test_pbr_f"))
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
