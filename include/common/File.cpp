#include "File.h"

std::string File::GetShaderPath(const std::string& name)
{
    return "../resource/shader/" + name + ".glsl";  //VS中默认的当前路径为vcxproj文件所在目录
}

std::string File::GetScenePath(const std::string& path)
{
    return "../resource/scene/" + path;
}

std::string File::GetTexturePath(const std::string& path)
{
    return "../resource/scene/texture/" + path;
}
