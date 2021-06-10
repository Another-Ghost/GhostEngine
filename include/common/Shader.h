#pragma once
#include "Prerequisite.h"

#include <fstream>
#include <sstream>
#include <iostream>

using std::cout;
using std::string;
using std::ifstream;
using std::stringstream;


class Shader
{


public:
	Shader(const string& vertex_path, const string& fragment_path, const string& geometry_path = "");

	// utility function for checking shader compilation/linking errors
	void CheckCompileErrors(GLuint shader, string type);

	void Use() const { glUseProgram(id); }

	void SetBool(const string& name, bool value) const { glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value); } //! is there a uniform bool type right now? No.

	void SetInt(const string& name, int value) const { glUniform1i(glGetUniformLocation(id, name.c_str()), value); }

	void SetFloat(const string& name, float value) const { glUniform1f(glGetUniformLocation(id, name.c_str()), value); }

	void SetVec2(const string& name, const vec2& value) const { glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); }

	void SetVec3(const string& name, const vec3& value) const { glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); }

	void SetVec4(const string& name, const vec4& value) const { glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); }

	void SetMat2(const string& name, const mat2& mat) const { glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

	void SetMat3(const string& name, const mat3& mat) const { glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

	void SetMat4(const string& name, const mat4& mat) const { glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

	unsigned int GetID() { return id; }

private:
	unsigned int id;
};

