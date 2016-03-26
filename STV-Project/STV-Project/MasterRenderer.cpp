#include "MasterRenderer.h"
#include <iostream>

#include "Camera.h"
#include "StaticShader.h"
#include "Entity.h"


MasterRenderer::MasterRenderer(GameManager* game_manager) : _GAME_MANAGER(game_manager)
{
	static_shader = new StaticShader();
}

MasterRenderer::~MasterRenderer()
{
	delete static_shader;
}

void MasterRenderer::Render() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	Camera::getMain()->buildViewMatrix();

	static_shader->Render();

	glutSwapBuffers();
	glutPostRedisplay();
}

