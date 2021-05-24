#pragma once
#include "common/World.h"

class BasicWorld : public World
{
public:
	BasicWorld(Renderer* ren, Repositry* rep) :World(ren, rep) { }

private:
	float delta_time;
};

