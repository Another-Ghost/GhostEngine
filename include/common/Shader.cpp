#include "Shader.h"
#include "Debug.h"
//#include "File.h"

Shader::Shader(const string& vertex_path_, const string& fragment_path_, const string& geometry_path_):
	vertex_path(vertex_path_), fragment_path(fragment_path_), geometry_path(geometry_path_)
{
	Generate();
}

void Shader::Generate()
{
	//1.retrieve the source code from filePath
	string vertex_code;
	string fragment_code;
	string geometry_code;
	ifstream vertex_shader_file;
	ifstream fragment_shader_file;
	ifstream geometry_shader_file;

	// ensure ifstream objects can throw exceptions
	vertex_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
	fragment_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
	geometry_shader_file.exceptions(ifstream::failbit | ifstream::badbit);

	cout << vertex_path << endl;
	cout << fragment_path << endl;

	try
	{
		vertex_shader_file.open(vertex_path);
		fragment_shader_file.open(fragment_path);
		stringstream vertex_shader_stream, fragment_shader_stream;
		vertex_shader_stream << vertex_shader_file.rdbuf();
		fragment_shader_stream << fragment_shader_file.rdbuf();
		vertex_shader_file.close();
		fragment_shader_file.close();

		vertex_code = vertex_shader_stream.str();
		fragment_code = fragment_shader_stream.str();
		if (geometry_path != "")
		{
			geometry_shader_file.open(geometry_path);
			stringstream geometry_shader_stream;
			geometry_shader_stream << geometry_shader_file.rdbuf();
			geometry_shader_file.close();
			geometry_code = geometry_shader_stream.str();
		}
	}
	catch (ifstream::failure& e)
	{
		cout << "ERROR<Shader>: Shader file is not loaded successfully \n";
	}

	const char* v_code = vertex_code.c_str();
	const char* f_code = fragment_code.c_str();
	//2. compile shaders
	unsigned int vertex;
	GLCall(vertex = glCreateShader(GL_VERTEX_SHADER));
	GLCall(glShaderSource(vertex, 1, &v_code, NULL)); //second parameter is the number of string of the source code
	GLCall(glCompileShader(vertex));
	CheckCompileErrors(vertex, "VERTEX");

	unsigned int fragment;
	GLCall(fragment = glCreateShader(GL_FRAGMENT_SHADER));
	GLCall(glShaderSource(fragment, 1, &f_code, NULL));
	GLCall(glCompileShader(fragment));
	CheckCompileErrors(fragment, "FRAGMENT");

	unsigned int geometry;
	if (geometry_path != "")
	{
		const char* g_code = geometry_code.c_str();
		GLCall(geometry = glCreateShader(GL_GEOMETRY_SHADER));
		GLCall(glShaderSource(geometry, 1, &g_code, NULL));
		GLCall(glCompileShader(geometry));
		CheckCompileErrors(geometry, "GEOMETRY");
	}

	//3. shader program
	GLCall(id = glCreateProgram());
	GLCall(glAttachShader(id, vertex));
	GLCall(glAttachShader(id, fragment));
	if (geometry_path != "")
		glAttachShader(id, geometry);
	GLCall(glLinkProgram(id));
	CheckCompileErrors(id, "PROGRAM");

	//4. delete the shaders
	GLCall(glDeleteShader(vertex));
	GLCall(glDeleteShader(fragment));
	if (geometry_path != "")
		glDeleteShader(geometry);
}

void Shader::Destroy()
{
	if (!id)
		return;

	glDeleteProgram(id);
	id = 0;
}

void Shader::Reload()
{
	Destroy();
	Generate();
}

void Shader::CheckCompileErrors(GLuint shader, string type)
{
	GLint success;
	int log_length = 0;
	if (type != "PROGRAM")
	{
		GLCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length));
		if (log_length)
		{
			GLchar* log_data = new GLchar[log_length];
			GLCall(glGetShaderInfoLog(shader, log_length, NULL, log_data));
			std::cout << "ERROR<Shader>: SHADER_COMPILATION_ERROR of type: " << type << "\n" << log_data << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		GLCall(glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &log_length));
		if (log_length)
		{
			GLchar* log_data = new GLchar[log_length];
			GLCall(glGetProgramInfoLog(shader, log_length, NULL, log_data));
			std::cout << "ERROR<Shader>: SHADER_LINKING_ERROR of type: " << type << "\n" << log_data << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}

}

string Shader::GetName(TextureUnit unit)
{
	switch (unit)
	{
	case TextureUnit::BASE_COLOR:
		return "basecolor_map";
		break;
	case TextureUnit::NORMAL:
		return "normal_map";
		break;
	case TextureUnit::AMBIENT_OCCLUSION:
		return "ao_map";
		break;
	case TextureUnit::METALNESS_ROUGHNESS:
		return "metalness_roughness_map";
		break;
	case TextureUnit::EMISSIVE:
		return "emissive_map";
		break;
	case TextureUnit::IRRADIANCE:
		return "irradiance_map";
		break;
	case TextureUnit::LIGHT_PREFILTER:
		return "light_prefilter_map";
		break;
	case TextureUnit::BRDF_LUT:
		return "brdf_lut";
		break;
	case TextureUnit::G_POSITION:
		return "g_position";
		break;
	case TextureUnit::G_NORMAL:
		return "g_normal";
		break;
	case TextureUnit::G_COLOR:
		return "g_color";
		break;
	case TextureUnit::SSAO:
		return "ssao";
		break;
	}
}


