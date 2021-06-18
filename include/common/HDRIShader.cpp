#include "HDRIShader.h"
#include "EquirectangularMap.h"
#include "RenderManager.h"

HDRIShader::HDRIShader(EquirectangularMap* hdr_map):
MVPShader(File::GetShaderPath("cubemap_vs"), File::GetShaderPath("equirectangular_to_cubemap_fs"))
{
	Use();
	SetInt("equirectangular_map", 0);
	
	SetProjectionMatrix(glm::perspective(glm::radians(90.f), 1.f, 0.1f, 10.f));

	
}


void HDRIShader::BindEquirectangulerMap(unsigned int tex_id)
{
	const vector<mat4>& capture_view_array = RenderManager::GetSingleton().capture_view_array;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_id);

}

unsigned int HDRIShader::RenderCubeMap(CubeMap* cube_map)
{


	return 0;
}
