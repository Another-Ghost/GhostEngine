#include "File.h"
#include <iostream>
#include <direct.h>
//#include <filesystem>

//#pragma warning(disable : 4996)

//namespace fs = std::filesystem;

std::string File::GetShaderPath(const std::string& name)
{
    return "../resources/shader/" + name + ".glsl";  //VS中默认的当前路径为vcxproj文件所在目录  //? 不加glsl
}

std::string File::GetScenePath(const std::string& path)
{
    return "../resources/scene/" + path;
}

std::string File::GetTexturePath(const std::string& path)
{
    
    return "../resources/texture/" + path;
}

std::string File::GetModelPath(const std::string& path) //? 改用std::filesystem::path
{
    //return "../resource/object/" + path;
    //static char const* env_root = getenv("LOGL_ROOT_PATH");
	//std::string path = "../resource/object/" + path;
	//char* buffer;
    std::string buffer;
    if ((buffer = _getcwd(NULL, 0)) == "")
    {
		perror("getcwd error");
    }
    else
    {
        std::cout << buffer << std::endl;



        //strchr(buffer, '/');
    }
    buffer = buffer.substr(0, buffer.find_last_of('\\'));
    char absulute[] = __FILE__;
    buffer = buffer + std::string("\\resources\\object\\") + path;
    buffer = ReplaceSubstring(buffer, "\\", "/");

    return buffer;
    //D:\Codes\Another - Ghost\GhostEngine\resource\object
  // return std::string("/") + path;
}

std::string File::ReplaceSubstring(std::string resource_str, std::string sub_str, std::string new_str)
{
    std::string::size_type pos = 0;
	while ((pos = resource_str.find(sub_str)) != std::string::npos)   //替换所有指定子串
	{
		resource_str.replace(pos, sub_str.length(), new_str);
	}
	return resource_str;
}
