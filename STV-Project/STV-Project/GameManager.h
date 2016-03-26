#pragma once
#include "MemoryListener.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"

#include "ModelLoader.h"
#include "MasterRenderer.h"
#include "TextureLoader.h"
#include "MarchingCube.h"
#include "Scene.h"

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
	MarchingCube* voxel_builder;

	/**
	Generates a singleton

	@returns GameManager singleton
	*/
	static GameManager* getMain()
	{
		static GameManager* MAIN = new GameManager();
		return MAIN;
	}
	
	void SetCurrentScene(Scene* scene);

	Scene* GetCurrentScene() { return _current_scene; }

private:
	Scene* _current_scene;

	void GLInit();
	void GameInit();
};


