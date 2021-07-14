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
	Shader(const string& vertex_path_, const string& fragment_path_, const string& geometry_path_ = "");

	~Shader() { Destroy(); }

	void Generate();

	void Destroy();

	virtual void Reload();	//？怎么改可以让子类重写的函数在结束时会自动调用基类的相应的被重写的函数


	// utility function for checking shader compilation/linking errors
	void CheckCompileErrors(GLuint shader, string type);

	void Use() const { glUseProgram(id); }

	void SetBool(const string& name, bool value) const { Use(); glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value); } //! is there a uniform bool type right now? No.

	void SetInt(const string& name, int value) const { Use(); glUniform1i(glGetUniformLocation(id, name.c_str()), value); }

	void SetFloat(const string& name, float value) const { Use(); glUniform1f(glGetUniformLocation(id, name.c_str()), value); }

	void SetVec2(const string& name, const vec2& value) const { Use(); glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); }

	void SetVec3(const string& name, const vec3& value) const { Use(); glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); }

	void SetVec4(const string& name, const vec4& value) const { Use(); glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); }

	void SetMat2(const string& name, const mat2& mat) const { Use(); glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

	void SetMat3(const string& name, const mat3& mat) const { Use(); glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

	void SetMat4(const string& name, const mat4& mat) const { Use(); glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

	unsigned int GetID() { return id; }

protected:
	unsigned int id;

private:
	string vertex_path;
	string fragment_path;
	string geometry_path;
};

