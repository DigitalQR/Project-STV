#include "GameManager.h"
#include <iostream>


int main(int argc, char **argv) 
{
	GameManager::getMain()->MainLoop();
	delete GameManager::getMain();

	cout << "Press enter to close.." << endl;
	cin.get();
	return 0;
}
