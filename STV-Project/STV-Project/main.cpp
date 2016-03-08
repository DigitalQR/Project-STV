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

	
	TestEntity* te = new TestEntity;
	te->AddComponent(new TestRotationComponent());


	Scene scene;
	scene.AddToScene(te);
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
