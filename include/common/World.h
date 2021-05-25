#pragma once
#include "Window.h"
#include "Renderer.h"
#include "Repositry.h"
#include "Camera.h"

class World
{
public:
	//World(Renderer* ren, Repositry* rep) :renderer(ren), repositry(rep) { }


	virtual void Update() = 0;

	Window* GetWindow() { return window; }
	void SetWindow(Window* win) { window = win; }
	Camera* GetCamera() { return camera; }
	void SetCamera(Camera* cam) { camera = cam; }//? 是否应该同步至其他类
	Renderer* GetRenderer() { return renderer; }
	void SetRenderer(Renderer* ren) { renderer = ren; }
	Repositry* GetRepositry() { return repositry; }
	void SetRepositry(Repositry* rep) { repositry = rep; }

protected:


	//virtual bool Init(Renderer* ren, Repositry* rep);
	Renderer* renderer;
	Repositry* repositry;
	Window* window;
	Camera* camera;

};

