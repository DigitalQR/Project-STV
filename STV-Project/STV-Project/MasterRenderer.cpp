#include "MasterRenderer.h"
#include <iostream>

#include "Camera.h"
#include "StaticShader.h"
#include "RenderQueue.h"
#include "Entity.h"

namespace Shaders 
{
	Shader* static_shader;
}
using namespace Shaders;

//FOR TESTING
Model* model;
Entity* entity;
Entity* entity1;
GLuint texture;

MasterRenderer::MasterRenderer(GameManager* game_manager) : _GAME_MANAGER(game_manager)
{
	static_shader = new StaticShader();

	vector<float> verts{
		-0.5, -0.5, 0.0,
		0.5, -0.5, 0.0,
		-0.5, 0.5, 0.0,
		0.5, 0.5, 0.0,
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

	//TESTING-------------------------------------------------------------------------------------------------------------------------------------
	model = _GAME_MANAGER->model_loader->CreateModel(verts, uvs, inds);
	model->texture = _GAME_MANAGER->texture_loader->LoadPNG("Res/world/grass_tile.png");
	entity = new Entity(model);
	entity1 = new Entity(model);
	entity1->transform.location = vec3(-1.0, 0.0, -0.1);
}

MasterRenderer::~MasterRenderer()
{
	delete static_shader;
	delete entity;
}

void MasterRenderer::Render() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.3, 0.3, 1.0);
	Camera::getMain()->buildViewMatrix();

	static_shader->PrepareShader();
	static_shader->PrepareModel(entity->model);
	static_shader->PrepareInstance(entity);
	glDrawElements(GL_TRIANGLES, entity->model->getIndiceCount(), GL_UNSIGNED_INT, 0);

	//TESTING-------------------------------------------------------------------------------------------------------------------------------------
	entity1->transform.rotation += vec3(0.0, 0.1, 0.0);
	entity1->transform.scale += 0.01;
	static_shader->PrepareInstance(entity1);
	glDrawElements(GL_TRIANGLES, entity1->model->getIndiceCount(), GL_UNSIGNED_INT, 0);

	glutSwapBuffers();
	glutPostRedisplay();
}

