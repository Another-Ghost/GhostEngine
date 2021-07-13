#include "BasicShader.h"
#include "BasicMaterial.h"

BasicShader::BasicShader(const string& vertex_path, const string& fragment_path, const string& geometry_path):
	MVPShader(vertex_path, fragment_path, geometry_path)
{
	texture_name_unit_map = {
	{"diffuse_map", 0},
	{"specular_map", 1},
	{"noarmal_map", 2}
	};

	for (const auto& name_unit : texture_name_unit_map)
	{
		Use();
		SetInt(name_unit.first, name_unit.second);
	}
}

void BasicShader::BindMaterial(BasicMaterial* material)
{
	if (material->diffuse_map_array.size())
	{
		glActiveTexture(GL_TEXTURE0 + texture_name_unit_map["diffuse_map"]);
		glBindTexture(GL_TEXTURE_2D, material->diffuse_map_array[0]->texture_id);
	}

	if (material->specular_map_array.size())
	{
		glActiveTexture(GL_TEXTURE0 + texture_name_unit_map["specular_map"]);
		glBindTexture(GL_TEXTURE_2D, material->specular_map_array[0]->texture_id);
	}

	if (material->normal_map_array.size())
	{
		glActiveTexture(GL_TEXTURE0 + texture_name_unit_map["noarmal_map"]);
		glBindTexture(GL_TEXTURE_2D, material->normal_map_array[0]->texture_id);
	}
}
