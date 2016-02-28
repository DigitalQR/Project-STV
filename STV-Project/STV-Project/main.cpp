#include "MemoryListener.h"
#include <iostream>

#include "GameManager.h"
#include "Model.h"
#include "TexturedModel.h"
#include "Entity.h"


using namespace std;


void Start() 
{
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

	Model* model = GameManager::getMain()->model_loader->CreateModel(verts, uvs, inds);
	GLuint texture = GameManager::getMain()->texture_loader->LoadPNG("Res/world/grass_tile.png");
	TexturedModel* tm = new TexturedModel(model, texture);

	Entity* entity = new Entity(tm);
	GameManager::getMain()->master_renderer->static_shader->AddForRender(entity);

	Entity* entity1 = new Entity(tm);
	entity1->location = vec3(0.0, 0.3, 0.2);
	GameManager::getMain()->master_renderer->static_shader->AddForRender(entity1);


	GameManager::getMain()->MainLoop();
	delete GameManager::getMain();

	delete model;
	delete entity;
	delete entity1;
	delete tm;
}

int main(int argc, char **argv)
{
	Start();
	
	cout << "Press enter to close.." << endl;
	cin.get();
	_CrtDumpMemoryLeaks();
	return 0;
}
