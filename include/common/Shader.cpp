#include "Shader.h"
#include "File.h"

Shader::Shader(const string& vertex_name, const string& fragment_name, const string& geometry_name)
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

	try
	{
		vertex_shader_file.open(File::GetShaderPath(vertex_name));
		fragment_shader_file.open(File::GetShaderPath(fragment_name));
		stringstream vertex_shader_stream, fragment_shader_stream;
		vertex_shader_stream << vertex_shader_file.rdbuf();
		fragment_shader_stream << fragment_shader_file.rdbuf();
		vertex_shader_file.close();
		fragment_shader_file.close();

		vertex_code = vertex_shader_stream.str();
		fragment_code = fragment_shader_stream.str();
		if (geometry_name != "")
		{
			geometry_shader_file.open(File::GetShaderPath(geometry_name));
			stringstream geometry_shader_stream;
			geometry_shader_stream << geometry_shader_file.rdbuf();
			geometry_shader_file.close();
			geometry_code = geometry_shader_stream.str();
		}
	}
	catch (ifstream::failure& e)
	{
		cout << "ERROR: Shader file is not loaded successfully \n";
	}

	const char* v_code = vertex_code.c_str();
	const char* f_code = fragment_code.c_str();
	//2. compile shaders
	unsigned int vertex;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &v_code, NULL); //second parameter is the number of string of the source code
	glCompileShader(vertex);
	CheckCompileErrors(vertex, "VERTEX");

	unsigned int fragment;
	fragment = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(fragment, 1, &f_code, NULL);
	glCompileShader(fragment);
	CheckCompileErrors(vertex, "FRAGMENT");
	
	unsigned int geometry;
	if (geometry_name != "")
	{
		const char* g_code = geometry_code.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &g_code, NULL);
		glCompileShader(geometry);
		CheckCompileErrors(geometry, "GEOMETRY");
	}

	//3. shader program
	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	if (geometry_name != "")
		glAttachShader(id, geometry);
	glLinkProgram(id);
	CheckCompileErrors(id, "PROGRAM");

	//4. delete the shaders
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometry_name != "")
		glDeleteShader(geometry);
	
}

void Shader::CheckCompileErrors(GLuint shader, string type)
{
	GLint success;
	int log_length = 0;
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
		if (log_length)
		{
			GLchar* log_data = new GLchar[log_length];
			glGetShaderInfoLog(shader, log_length, NULL, log_data);
			std::cout << "ERROR: SHADER_COMPILATION_ERROR of type: " << type << "\n" << log_data << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
		if (log_length)
		{
			GLchar* log_data = new GLchar[log_length];
			glGetShaderInfoLog(shader, log_length, NULL, log_data);
			std::cout << "ERROR: SHADER_LINKING_ERROR of type: " << type << "\n" << log_data << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}

}
