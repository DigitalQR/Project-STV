#include "MemoryListener.h"
#include <iostream>

#include "GameManager.h"
#include "Model.h"
#include "TexturedModel.h"
#include "Terrain.h"

#include "TestEntity.h"
#include "TestRotationComponent.h"
#include "Scene.h"

using namespace std;


void Start() 
{
	GameManager::getMain();


	TestEntity* te1 = new TestEntity;
	te1->ReadyUp();
	te1->location = vec3(0, 90, 0);
	te1->AddComponent(new TestRotationComponent());
	te1->AddComponent(new ActualRotationComponent());

	Terrain* terrain = new Terrain(2787);

	Scene scene;
	scene.AddToScene(te1);
	scene.SetTerrain(terrain);
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
