#include "MemoryListener.h"
#include <iostream>

#include "GameManager.h"
#include "Model.h"
#include "TexturedModel.h"
#include "Terrain.h"

#include "PlayerController.h"
#include "DayCycleController.h"
#include "Scene.h"

using namespace std;


void Start() 
{
	GameManager::getMain();

	Entity* player = new Entity();
	player->location = vec3(0, 40, 0);
	player->AddComponent(new PlayerController());

	Terrain* terrain = new Terrain(2787);
	Entity* day_cycle_controller = new Entity();
	day_cycle_controller->AddComponent(new DayCycleController());

	Scene* scene = new Scene();
	scene->AddToScene(player);
	scene->AddToScene(day_cycle_controller);
	scene->SetTerrain(terrain);
	GameManager::getMain()->SetCurrentScene(scene);

	GameManager::getMain()->MainLoop();
	GameManager::getMain()->SetCurrentScene(nullptr);
	delete scene;
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
