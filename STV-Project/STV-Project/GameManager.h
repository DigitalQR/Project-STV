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
	void MainLoop();

	MasterRenderer* master_renderer;
	ModelLoader* model_loader;

	static GameManager* getMain()
	{
		static GameManager* MAIN = new GameManager();
		return MAIN;
	}

private:
	void GLInit();
	void GameInit();

};


