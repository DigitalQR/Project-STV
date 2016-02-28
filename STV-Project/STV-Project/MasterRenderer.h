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

	MasterRenderer(GameManager* game_manager);
	~MasterRenderer();
	void Render();

private:
	const GameManager* _GAME_MANAGER;
};



