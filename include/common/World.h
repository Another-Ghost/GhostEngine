#pragma once

class Repositry;

class Renderer;

class World
{
public:
	World(Renderer* ren, Repositry* rep) :renderer(ren), repositry(rep) { }

	virtual bool Update();

protected:


	//virtual bool Init(Renderer* ren, Repositry* rep);
	Renderer* renderer;
	Repositry* repositry;



};

