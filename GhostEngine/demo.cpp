#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "BasicWorld.h"


int main()
{
	//BasicRenderer renderer; //? 在堆中生成更好？
	//BasicRepositry repositry;
	BasicWorld world;

	world.Update();

	return 0;
}