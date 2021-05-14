#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
using namespace glm;


class Shader
{
	unsigned int id;

	Shader(const string& vertex_path, const string& fragment_path, const string& geometry_path = "");

	void CheckCompileErrors(GLuint shader, string type);

	void Use() const { glUseProgram(id); }

	void SetBool(const string& name, bool value) const { glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value); } //! is there a uniform bool type right now? No.

	void SetInt(string& name, int value) const { glUniform1i(glGetUniformLocation(id, name.c_str()), value); }

	void SetFloat(const string& name, float value) const { glUniform1f(glGetUniformLocation(id, name.c_str()), value); }

	void SetVec2(const string& name, const vec2& value) const{glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);}


};

