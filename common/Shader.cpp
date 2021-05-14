#include "Shader.h"

Shader::Shader(const string& vertex_path, const string& fragment_path, const string& geometry_path)
{
	//1.retrieve the source code from filePath
	const char* vertex_code;
	const char* fragment_code;
	const char* geometry_code;
	ifstream vertex_shader_file;
	ifstream fragment_shader_file;
	ifstream geometry_shader_file;

	// ensure ifstream objects can throw exceptions
	vertex_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
	fragment_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
	geometry_shader_file.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		vertex_shader_file.open(vertex_path);
		fragment_shader_file.open(fragment_path);
		stringstream vertex_shader_stream, fragment_shader_stream;
		vertex_shader_stream << vertex_shader_file.rdbuf();
		fragment_shader_stream << fragment_shader_file.rdbuf();
		vertex_shader_file.close();
		fragment_shader_file.close();

		vertex_code = vertex_shader_stream.str().c_str();
		fragment_code = fragment_shader_stream.str().c_str();
		if (geometry_path != "")
		{
			geometry_shader_file.open(geometry_path);
			stringstream geometry_shader_stream;
			geometry_shader_stream << geometry_shader_file.rdbuf();
			geometry_shader_file.close();
			geometry_code = geometry_shader_stream.str().c_str();
		}
	}
	catch (ifstream::failure& e)
	{
		cout << "ERROR: Shader file is not loaded successfully" << endl;
	}

	//2. compile shaders
	unsigned int vertex;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertex_code, NULL); //second parameter is the number of string of the source code
	glCompileShader(vertex);
	CheckCompileErrors(vertex, "VERTEX");

	unsigned int fragment;
	fragment = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(fragment, 1, &fragment_code, NULL);
	glCompileShader(fragment);
	CheckCompileErrors(vertex, "FRAGMENT");
	
	unsigned int geometry;
	if (geometry_path != "")
	{
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &geometry_code, NULL);
		glCompileShader(geometry);
		CheckCompileErrors(geometry, "GEOMETRY");
	}

	//3. shader program
	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	if (geometry_path != "")
		glAttachShader(id, geometry);
	glLinkProgram(id);
	CheckCompileErrors(id, "PROGRAM");

	//4. delete the shaders
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometry_path != "")
		glDeleteShader(geometry);
	
}

void Shader::CheckCompileErrors(GLuint shader, string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR: SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);	//not relating to linker, it's just means like if vertex shader could link to fragment shader
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR: PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}

}
