#include "MasterRenderer.h"
#include <iostream>

#include "Camera.h"
#include "StaticShader.h"
#include "TerrainShader.h"
#include "Entity.h"


MasterRenderer::MasterRenderer(GameManager* game_manager) : _GAME_MANAGER(game_manager)
{
	static_shader = new StaticShader();
	terrain_shader = new TerrainShader();
}

MasterRenderer::~MasterRenderer()
{
	delete static_shader;
	delete terrain_shader;
}

void MasterRenderer::Render() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	Camera::getMain()->buildViewMatrix();

	terrain_shader->Render();
	static_shader->Render();

	glutSwapBuffers();
	glutPostRedisplay();
}

