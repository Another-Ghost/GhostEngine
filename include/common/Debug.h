#pragma once
//#include "Prerequisite.h"
#include <glad/glad.h>
#include <iostream>

using std::cout;
//add #debug
#define ASSERT(x) if(!(x)) __debugbreak();	//specific to MSVC, it won't work in clang or gcc, intrinsic in compiler
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))	// #x means turning x into string 
//__FILE__ & __LINE__ is not an intrinsic, it should be supported by all compilers
	

static void GLClearError()
{
	//while (glGetError());
	while (glGetError() != GL_NO_ERROR);
}

static void GLCheckError()
{
	while (GLenum error = glGetError())
	{
		cout << "[OpenGL Error] (" << std::hex << error << ") \n";
	}
}

//@parameters
//function: the function we tried to call with all of its arguments and everything
//file: C++ source file where this function was actually call from
//file: which line the function is in 

static bool GLLogCall(const char* function, const char* file, int line)	
{
	while (GLenum error = glGetError())
	{
		cout << "[OpenGL Error] (" << std::hex << error << "): " << function << " " << file << " " << line << "\n";
		return false;
	}

	return true;
}


