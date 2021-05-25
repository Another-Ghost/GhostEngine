#include "BasicWorld.h"
//#include "BasicRenderer.h"
//#include "BasicRepositry.h"

int main()
{
	//BasicRenderer renderer; //? 在堆中生成更好？
	//BasicRepositry repositry;
	BasicWorld world;

	world.Update();

	return 0;
}