#pragma once
#include "MemoryListener.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"

#include "ModelLoader.h"
#include "MasterRenderer.h"
#include "TextureLoader.h"

#include <iostream>
#include <list>

class MasterRenderer;

class GameManager
{
public:
	GameManager();
	~GameManager();
	/**
	Activates glut's main loop

	*/
	void MainLoop();

	MasterRenderer* master_renderer;
	ModelLoader* model_loader;
	TextureLoader* texture_loader;

	/**
	Generates a singleton

	@returns GameManager singleton
	*/
	static GameManager* getMain()
	{
		static GameManager* MAIN = new GameManager();
		return MAIN;
	}

private:
	void GLInit();
	void GameInit();

};


