#include "PBRShader.h"
#include "Light.h"
#include "Camera.h"
#include "PBRMaterial.h"

PBRShader::PBRShader(const string& vertex_path, const string& fragment_path, const string& geometry_path):
	MVPShader(vertex_path, fragment_path, geometry_path)
{
	texture_name_unit_map = {	//? 改成用texture类全局的映射
		{"basecolor_map", 0},
		{"normal_map", 1},
		{"metalness_map", 2},
		{"roughness_map", 3},
		{"ao_map", 4},
		{"irradiance_map", 5}, //environment diffuse irradiance map
		{"prefilter_map", 6}, //environment specular Split Sum 1st stage 
		{"brdf_lut", 7}, //environment specular Split Sum 2nd stage 
		{"ssda_lut", 8},
		{"metalness_roughness_map", 9},
		{"emissive_map", 10}
	}; //environment specular Split Sum 2nd stage

	for (const auto& name_unit : texture_name_unit_map)
	{
		Use();
		SetInt(name_unit.first, name_unit.second);
	}
}

void PBRShader::Reload()
{
	Shader::Reload();
	for (const auto& name_unit : texture_name_unit_map)
	{
		Use();
		SetInt(name_unit.first, name_unit.second);
	}
}



void PBRShader::SetCameraPosition(const vec3& pos)
{
	Use();
	cam_pos = pos;
	SetVec3("cam_pos", cam_pos);
}

void PBRShader::SetCamera(Camera* camera)
{
	Use();
	SetProjectionMatrix(camera->PerspectiveMatrix());
	SetViewMatrix(camera->ViewMatrix());
	SetCameraPosition(camera->GetPosition());
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

void PBRShader::BindMaterial(PBRMaterial* material)
{
	glActiveTexture(GL_TEXTURE0 + texture_name_unit_map["basecolor_map"]);
	glBindTexture(GL_TEXTURE_2D, material->basecolor_map->id);

	glActiveTexture(GL_TEXTURE0 + texture_name_unit_map["normal_map"]);
	glBindTexture(GL_TEXTURE_2D, material->normal_map->id);

	if (material->ao_map)
	{
		glActiveTexture(GL_TEXTURE0 + texture_name_unit_map["ao_map"]);
		glBindTexture(GL_TEXTURE_2D, material->ao_map->id);
	}
	else
	{
		glActiveTexture(GL_TEXTURE0 + texture_name_unit_map["ao_map"]);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if (!material->metalness_roughness_map)
	{
		//if (material->metalness_map && material->roughness_map)
			material->CreateMetalnessRoughnessMap();
	}
	glActiveTexture(GL_TEXTURE0 + texture_name_unit_map["metalness_roughness_map"]);
	glBindTexture(GL_TEXTURE_2D, material->metalness_roughness_map->id);

	if (material->emissive_map)
	{
		glActiveTexture(GL_TEXTURE0 + texture_name_unit_map["emissive_map"]);
		glBindTexture(GL_TEXTURE_2D, material->emissive_map->id);
	}
	else
	{
		glActiveTexture(GL_TEXTURE0 + texture_name_unit_map["emissive_map"]);
		glBindTexture(GL_TEXTURE_2D, 0);

	}

#ifdef DEBUG_MODE
	if (material->metalness_map)
	{
		glActiveTexture(GL_TEXTURE0 + texture_name_unit_map["metalness_map"]);
		glBindTexture(GL_TEXTURE_2D, material->metalness_map->id);
	}

	if (material->roughness_map)
	{
		glActiveTexture(GL_TEXTURE0 + texture_name_unit_map["roughness_map"]);
		glBindTexture(GL_TEXTURE_2D, material->roughness_map->id);
	}
#endif

}


void PBRShader::BindBaseColorMap(unsigned int tex_id)
{
	//Use();	//? Don't need this?
	glActiveTexture(GL_TEXTURE0 + texture_name_unit_map["basecolor_map"]);
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
