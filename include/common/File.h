#pragma once
#include <string>


class File
{
public:
	static std::string GetShaderPath(const std::string& name);

	static std::string GetScenePath(const std::string& path);

	static std::string GetTexturePath(const std::string& path);
};

