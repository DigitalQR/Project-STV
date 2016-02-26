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
GLuint texture;
GLuint texture1;

MasterRenderer::MasterRenderer(GameManager* game_manager) : _GAME_MANAGER(game_manager)
{
	static_shader = new StaticShader();

	vector<float> verts{
		0.0, 0.0, 0.5,
		1.0, 0.0, 0.5,
		0.0, 1.0, 0.5,
		1.0, 1.0, 0.5,
	}; 
	vector<float> uvs{
		0.0, 1.0,
		1.0, 1.0,
		0.0, 0.0,
		1.0, 0.0,
	};
	vector<unsigned int> inds{
		0,1,2,
		1,3,2
	};
	model = _GAME_MANAGER->model_loader->CreateModel(verts, uvs, inds);

	texture = _GAME_MANAGER->texture_loader->LoadPNG("Res/test.png");
	texture1 = _GAME_MANAGER->texture_loader->LoadPNG("Res/test2.png");
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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawElements(GL_TRIANGLES, model->getIndiceCount(), GL_UNSIGNED_INT, 0);

	glutSwapBuffers();
	glutPostRedisplay();
}

