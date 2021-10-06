#pragma once
#include "BasicDependencies.h"

class RenderBuffer
{
public:
	GLuint id;

	int width;
	int height;

	RenderBuffer();

	RenderBuffer(int _w, int _h);

	void Resize(int _w, int _h);
};

