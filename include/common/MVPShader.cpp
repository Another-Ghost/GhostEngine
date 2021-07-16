#include "MVPShader.h"
#include "RenderManager.h"

MVPShader::MVPShader(const string& vertex_path, const string& fragment_path, const string& geometry_path /*= ""*/):
	Shader(vertex_path, fragment_path, geometry_path)
{
	
}

void MVPShader::SetPointLight()
{
	GLuint pos_index = glGetProgramResourceIndex(id, GL_SHADER_STORAGE_BLOCK, "LightPosition");
	glShaderStorageBlockBinding(id, pos_index, 1);
	GLuint color_index = glGetProgramResourceIndex(id, GL_SHADER_STORAGE_BLOCK, "LightColor");
	glShaderStorageBlockBinding(id, color_index, 2);
}
