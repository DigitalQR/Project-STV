#pragma once
#include "MemoryListener.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"

#include "GameManager.h"
#include "Shader.h"

class GameManager;

class MasterRenderer
{
public:
	Shader* static_shader;
	Shader* terrain_shader;
	Shader* hand_shader;

	MasterRenderer(GameManager* game_manager);
	~MasterRenderer();
	void Render();
	void ClearAllRenderQueues() 
	{
		static_shader->ClearRenderQueue();
	}

private:
	const GameManager* _GAME_MANAGER;
};



