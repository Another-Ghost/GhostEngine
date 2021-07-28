#pragma once
#include "Prerequisite.h"

#include <fstream>
#include <sstream>
#include <iostream>

using std::ifstream;
using std::stringstream;


//enum class TextureUnit
//{
//	BASE_COLOR = 0,
//	NORMAL = 1,
//	AMBIENT_OCCLUSION = 2, 
//	METALNESS_ROUGHNESS = 3,
//	EMISSIVE = 4,
//	IRRADIANCE = 5,
//	LIGHT_PREFILTER = 6,
//	BRDF_LUT = 7,
//	G_POSITION = 8,
//	G_NORMAL = 9,
//	G_COLOR = 10, 
//	SSAO = 11,
//};

struct TextureUnit
{
	string name;
	int number;

	//TextureUnit(const string& name_, int number_) : name(name_), number(number_) { }

	static const TextureUnit basecolor_map;
	static const TextureUnit normal_map;
	static const TextureUnit ao_map;
	static const TextureUnit metalness_roughness_map;
	static const TextureUnit emissive_map;
	static const TextureUnit irradiance_map;
	static const TextureUnit light_prefilter_map;
	static const TextureUnit brdf_lut;
	static const TextureUnit ssao;
	static const TextureUnit g_world_position;
	static const TextureUnit g_world_normal;
	static const TextureUnit g_basecolor;
	static const TextureUnit g_view_position;
	static const TextureUnit g_view_normal;
	static const TextureUnit g_ao_roughness_metalness;
	static const TextureUnit g_emissive;
	static const TextureUnit g_color;


	//? static void Reset();
	static void Bind2DTexture(const TextureUnit& unit, PlaneTexture* texture);
	static void BindCubemapTexture(const TextureUnit& unit, CubeMap* texture);
};

class Shader
{

public:


	Shader(const string& vertex_path_, const string& fragment_path_, const string& geometry_path_ = "");

	~Shader() { Destroy(); }

	void Generate();

	void Destroy();

	void Reload();	//����ô�Ŀ�����������д�ĺ����ڽ���ʱ���Զ����û������Ӧ�ı���д�ĺ���

	//don't forget to call parent's initialize if it's reloaded
	virtual void Initialize(bool b_reload = false);	

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


	//static int GetNumber(TextureUnit unit) { return static_cast<int>(unit); }
	//static string GetName(TextureUnit unit);
	void UseTextureUnit(TextureUnit unit) { Use(); SetInt(unit.name, unit.number); }
	//void Bind2DTexture(const TextureUnit& unit, Texture* texture);
	//void BindCubemapTexture(const TextureUnit& unit, CubeMap* texture);

protected:
	unsigned int id;

private:
	string vertex_path;
	string fragment_path;
	string geometry_path;
};

