#include "PBRShader.h"
#include "File.h"

PBRShader::PBRShader() :
	MVPShader(File::GetShaderPath("pbr_vs"), File::GetShaderPath("pbr_fs"))
{
	texture_name_unit_map = { 
		{"albedo_map", 0}, 
		{"normal_map", 1},
		{"metalness_map", 2},
		{"roughness_map", 3}, 
		{"ao_map", 4}, 
		{"irradiance_map", 5}, //environment diffuse irradiance map
		{"prefilter_map", 6}, //environment specular Split Sum 1st stage 
		{"brdf_lut", 7} 
	}; //environment specular Split Sum 2nd stage

	for (const auto& name_unit : texture_name_unit_map)
	{
		Use();
		SetInt(name_unit.first, name_unit.second);
	}


	
}

void PBRShader::SetRoughness(float roughness_)
{
	Use();
	roughness = roughness_;
	SetFloat("roughness", roughness);
}

void PBRShader::SetCameraPosition(const vec3& pos)
{
	Use();
	cam_pos = pos;
	SetVec3("cam_pos", cam_pos);
}

void PBRShader::SetPointLightArray(const vector<Light*>& light_array)
{
	Use();
	for (int i = 0; i < light_array.size(); ++i)
	{
		SetVec3("light_position_array[" + std::to_string(i) + "]", light_array[i]->postion);
		SetVec3("light_color_array[" + std::to_string(i) + "]", light_array[i]->color * light_array[i]->intensity);
	}
}


void PBRShader::BindAlbedoMap(unsigned int tex_id)
{
	Use();
	glActiveTexture(GL_TEXTURE0 + texture_name_unit_map["albedo_map"]);
	glBindTexture(GL_TEXTURE_2D, tex_id);
}

void PBRShader::BindNormalMap(unsigned int tex_id)
{
	glActiveTexture(GL_TEXTURE0 + texture_name_unit_map["normal_map"]);
	glBindTexture(GL_TEXTURE_2D, tex_id);
}

void PBRShader::BindMetalnessMap(unsigned int tex_id)
{
	glActiveTexture(GL_TEXTURE0 + texture_name_unit_map["metalness_map"]);
	glBindTexture(GL_TEXTURE_2D, tex_id);
}

void PBRShader::BindRoughnessMap(unsigned int tex_id)
{
	glActiveTexture(GL_TEXTURE0 + texture_name_unit_map["roughness_map"]);
	glBindTexture(GL_TEXTURE_2D, tex_id);
}

void PBRShader::BindAmbientOcclusionMap(unsigned int tex_id)
{
	glActiveTexture(GL_TEXTURE0 + texture_name_unit_map["ao_map"]);
	glBindTexture(GL_TEXTURE_2D, tex_id);
}

void PBRShader::BindEnvDiffuseIrradianceMap(unsigned int tex_id)
{
	glActiveTexture(GL_TEXTURE0 + texture_name_unit_map["irradiance_map"]);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex_id);
}

void PBRShader::BindEnvSpecularPrefilterMap(unsigned int tex_id)
{
	glActiveTexture(GL_TEXTURE0 + texture_name_unit_map["prefilter_map"]);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex_id);
}

void PBRShader::BindEnvSpecularBRDFLUT(unsigned int tex_id)
{
	glActiveTexture(GL_TEXTURE0 + texture_name_unit_map["brdf_lut"]);
	glBindTexture(GL_TEXTURE_2D, tex_id);
}
