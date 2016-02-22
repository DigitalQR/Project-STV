#include "GameManager.h"
#include <iostream>

static GameManager* main_manager;

int main(int argc, char **argv) 
{
	main_manager = new GameManager();
	main_manager->Setup(argc, argv);
	main_manager->MainLoop();
	delete main_manager;

	cout << "Press enter to close.." << endl;
	std::cin.get();
	return 0;
}
