#pragma once
#include "MemoryListener.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"

#include "ModelLoader.h"
#include "MasterRenderer.h"
#include "TextureLoader.h"
#include "MarchingCube.h"
#include "Scene.h"
#include "StatisticCounter.h"
#include "Timer.h"

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

	Timer update_timer;
	float LERP_time = 0;

	float GetLERPTime()
	{
		return LERP_time;
	}
	float GetDynamicLERPTime()
	{
		return (update_timer.GetElapsedTime()*1.0f) / (UPDATE_SLEEP * 1.0f);
	}

	void LogicUpdate();
	void VisualUpdate();

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

	vec3 sky_colour;
	vec3 fog_colour;
	float fog_density;
	float fog_gradient;

private:
	Scene* _current_scene = nullptr;
	const unsigned int UPDATE_SLEEP = 1000/30;
	bool running = false;
	bool logic_running = false;

	void GLInit();
	void GameInit();

	void LogicLoop();
	void VisualLoop();
};


