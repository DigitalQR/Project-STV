#include "MemoryListener.h"
#include <iostream>

#include "GameManager.h"
#include "Model.h"
#include "TexturedModel.h"

#include "TestEntity.h"
#include "TestRotationComponent.h"


using namespace std;


void Start() 
{
	GameManager::getMain();

	
	TestEntity te;
	te.AddComponent(new TestRotationComponent());
	GameManager::getMain()->AddToGame(&te);

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
