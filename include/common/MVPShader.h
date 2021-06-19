#pragma once
#include "Shader.h"
#include "File.h"
#include "WindowManager.h"

class MVPShader : public Shader
{
public:
    MVPShader(const string& vertex_path, const string& fragment_path, const string& geometry_path = "");

    void SetModelMatrix(const mat4& matrix) { Use(); model_matrix = matrix; SetMat4("model", matrix); }

    void SetViewMatrix(const mat4& matrix) { Use(); view_matrix = matrix; SetMat4("view", matrix); }

    void SetProjectionMatrix(const mat4& matrix) { Use(); projection_matrix = matrix; SetMat4("projection", matrix); }


protected:
	mat4 model_matrix;
	mat4 view_matrix;
	mat4 projection_matrix;

	map<string, int> texture_name_unit_map;	//名字和纹理单元（texture unit）的映射
};

