#pragma once
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"

#include "Shader.h"
#include "ModelLoader.h"
#include "MasterRenderer.h"

#include <iostream>

using namespace std;
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


