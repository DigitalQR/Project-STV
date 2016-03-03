#include "MemoryListener.h"
#include <iostream>

#include "GameManager.h"
#include "Model.h"
#include "TexturedModel.h"
#include "Entity.h"

#include "TestRotationComponent.h"
#include "OBJLoader.h"

using namespace std;


void Start() 
{
	GameManager::getMain();

	map<string, Model*> model_map = OBJLoader::LoadOBJ("Res/Creatures/Player/T-Pose.obj");

	Model* model = model_map["Torso"];
	GLuint texture = GameManager::getMain()->texture_loader->LoadPNG("Res/Creatures/Player/stv_charpal.png");
	TexturedModel* tm = new TexturedModel(model, texture);

	Entity* entity = new Entity(tm);
	entity->AddComponent(new TestRotationComponent());
	GameManager::getMain()->master_renderer->static_shader->AddForRender(entity);

	Entity* entity1 = new Entity(tm);
	entity1->AddComponent(new TestRotationComponent());
	//entity1->location = vec3(0.0, 1.0, 0);
	//GameManager::getMain()->master_renderer->static_shader->AddForRender(entity1);

	//entity->setParent(entity1);

	GameManager::getMain()->MainLoop();
	delete GameManager::getMain();

	for (map<string, Model*>::iterator it = model_map.begin(); it != model_map.end(); it++)
	{
		delete it->second;
	}
	
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
