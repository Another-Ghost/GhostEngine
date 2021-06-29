#pragma once
#include "MVPShader.h"
#include "File.h"

union Uniform
{
	int i;
	float f;
	mat4 mat_4;
};

class PBRShader : public MVPShader
{
	Uniform u;

public:
	PBRShader(const string& vertex_path = File::GetShaderPath("pbr_vs"), const string& fragment_path = File::GetShaderPath("pbr_fs"), const string& geometry_path = "");

	void Reload() override;


	void SetCameraPosition(const vec3& pos);

	void SetCamera(Camera* camera);

	void SetPointLightArray(const vector<Light*>& light_array);	//? 改为传入PointLight class

	void BindAlbedoMap(unsigned int tex_id);

	void BindNormalMap(unsigned int tex_id);

	void BindMetalnessMap(unsigned int tex_id);

	void BindRoughnessMap(unsigned int tex_id);

	void BindAmbientOcclusionMap(unsigned int tex_id);

	void BindEnvDiffuseIrradianceMap(unsigned int tex_id);

	void BindEnvSpecularPrefilterMap(unsigned int tex_id);

	void BindEnvSpecularBRDFLUT(unsigned int tex_id);

private:



	vec3 cam_pos;

	vector<vec3> pointlight_position_array;
	vector<vec3> pointlight_color_array;
};

