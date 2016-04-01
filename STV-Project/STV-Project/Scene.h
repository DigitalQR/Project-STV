#pragma once
#include "Entity.h"
#include "Terrain.h"
#include <list>

using namespace std;

class Scene
{
public:
	~Scene() 
	{
		for (Entity* entity : _entities)
			delete entity;

		delete _terrain;
	};

	void VisualUpdate() 
	{	
		for (Entity* entity : _entities)
			entity->VisualUpdate();

		_terrain->VisualUpdate();
	}

	void LogicUpdate() 
	{
		for (Entity* entity : _entities) 
		{
			entity->TransformUpdate();
			entity->LogicUpdate();
		}
	}

	void AddContentForRender() 
	{
		for (Entity* entity : _entities)
			entity->AddElementsForRender();
	}

	void AddToScene(Entity* entity)
	{
		_entities.push_back(entity);
		entity->Start();
	}

	void SetTerrain(Terrain* terrain) 
	{
		_terrain = terrain;
	}

	void AttachScene() 
	{
		for (Entity* entity : _entities)
			entity->AddElementsForRender();
	}

private:
	list<Entity*> _entities;
	Terrain* _terrain;

};

