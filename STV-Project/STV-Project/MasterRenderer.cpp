#include "MasterRenderer.h"
#include <iostream>
#include "StaticShader.h"

namespace Shaders 
{
	Shader* static_shader;
}
using namespace Shaders;

//FOR TESTING
Model* model;

MasterRenderer::MasterRenderer(GameManager* game_manager) : _GAME_MANAGER(game_manager)
{
	static_shader = new StaticShader();

	vector<float> verts{
		0.0, 0.0, 0.5,
		1.0, 0.0, 0.5,
		0.0, 1.0, 0.5,
	};
	model = _GAME_MANAGER->model_loader->CreateModel(verts);
}

MasterRenderer::~MasterRenderer()
{
	delete static_shader;
}

void MasterRenderer::Render() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.3, 0.3, 1.0);

	static_shader->PrepareShader();
	glBindVertexArray(model->getVAO());
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glutSwapBuffers();
	glutPostRedisplay();
}

