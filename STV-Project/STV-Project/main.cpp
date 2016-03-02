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
	Model* model = OBJLoader::LoadOBJ("Res/TestOBJ.obj")["Box002"];
	GLuint texture = GameManager::getMain()->texture_loader->LoadPNG("Res/world/grass_tile.png");
	TexturedModel* tm = new TexturedModel(model, texture);

	Entity* entity = new Entity(tm);
	entity->AddComponent(new TestRotationComponent());
	//entity->pivot = vec3(0.5, 0.5, 0);
	GameManager::getMain()->master_renderer->static_shader->AddForRender(entity);

	Entity* entity1 = new Entity(tm);
	entity1->AddComponent(new TestRotationComponent());
	//entity1->location = vec3(0.0, 1.0, 0);
	GameManager::getMain()->master_renderer->static_shader->AddForRender(entity1);

	entity->setParent(entity1);

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
