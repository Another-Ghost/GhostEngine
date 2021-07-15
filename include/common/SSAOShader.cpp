#include "SSAOShader.h"

SSAOShader::SSAOShader(const string& vertex_path /*= File::GetShaderPath("basic_v")*/, const string& fragment_path /*= File::GetShaderPath("ssao_f")*/, const string& geometry_path /*= ""*/):
	MVPShader(vertex_path, fragment_path, geometry_path)
{

}
