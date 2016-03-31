#include "MemoryListener.h"
#include <iostream>

#include "GameManager.h"
#include "Model.h"
#include "TexturedModel.h"

#include "TestEntity.h"
#include "TestRotationComponent.h"
#include "Scene.h"

using namespace std;


void Start() 
{
	GameManager::getMain();


	TestEntity* te = new TestEntity2;
	te->ReadyUp();
	te->AddComponent(new TestRotationComponent());

	TestEntity* te1 = new TestEntity;
	te1->ReadyUp();
	te1->location = vec3(0, 90, 0);
	te1->AddComponent(new ActualRotationComponent());


	Scene scene;
	scene.AddToScene(te);
	scene.AddToScene(te1);
	GameManager::getMain()->SetCurrentScene(&scene);

	GameManager::getMain()->MainLoop();

	delete GameManager::getMain();
}

int main(int argc, char **argv)
{
	Start();
	
	cout << "Press enter to close.." << endl;
	cin.get();
	_CrtDumpMemoryLeaks();
	return 0;
}
