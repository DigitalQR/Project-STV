#include "MemoryListener.h"
#include <iostream>

#include "GameManager.h"
#include "Model.h"
#include "TexturedModel.h"
#include "Entity.h"

#include "TestRotationComponent.h"
#include "OBJLoader.h"
#include "Element3D.h"

using namespace std;


void Start() 
{
	GameManager::getMain();

	map<string, Model*> model_map = OBJLoader::LoadOBJ("Res/Creatures/Player/T-Pose.obj");

	Model* model = model_map["Torso"];
	GLuint texture = GameManager::getMain()->texture_loader->LoadPNG("Res/Creatures/Player/stv_charpal.png");
	TexturedModel* tm = new TexturedModel(model, texture);

	Element3D element(tm);

	GameManager::getMain()->master_renderer->static_shader->AddForRender(&element);

	Entity entity;
	entity.AddComponent(new TestRotationComponent);
	entity.GetComponent<TestRotationComponent>();


	GameManager::getMain()->MainLoop();

	for (map<string, Model*>::iterator it = model_map.begin(); it != model_map.end(); it++)
	{
		delete it->second;
	}

	delete GameManager::getMain();
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
