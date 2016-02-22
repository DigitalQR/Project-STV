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
	void Setup(int argc, char** argv);
	~GameManager();
	void MainLoop();
	MasterRenderer* master_renderer;
	ModelLoader* model_loader;

private:
	void GLInit();
	void GameInit();

};


