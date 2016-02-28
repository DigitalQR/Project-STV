#include "MemoryListener.h"

#include <iostream>

using namespace std;

#include "GameManager.h"
#include "Model.h"
#include "Entity.h"

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
	model->texture = GameManager::getMain()->texture_loader->LoadPNG("Res/world/grass_tile.png");
	Entity* entity = new Entity(model);
	GameManager::getMain()->master_renderer->static_shader->AddForRender(entity);

	GameManager::getMain()->MainLoop();
	delete GameManager::getMain();

	delete model;
	delete entity;
}

int main(int argc, char **argv)
{
	Start();
	
	cout << "Press enter to close.." << endl;
	cin.get();
	_CrtDumpMemoryLeaks();
	return 0;
}
