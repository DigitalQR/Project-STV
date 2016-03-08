#pragma once
#include "MemoryListener.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"

#include "ModelLoader.h"
#include "MasterRenderer.h"
#include "TextureLoader.h"
#include "Entity.h"

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

	void AddToGame(Entity* entity) 
	{
		_entities.push_back(entity);
		entity->Start();
		entity->AddElementsForRender();
	}

	void EntityVisualUpdates() 
	{
		for (Entity* entity : _entities)
			entity->VisualUpdate();
	}

private:
	list<Entity*> _entities;

	void GLInit();
	void GameInit();
};


