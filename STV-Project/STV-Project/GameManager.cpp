#include "GameManager.h"
#include "Model.h"
#include <vector>

namespace Game{
	MasterRenderer* master_renderer;
}

void RenderScene()
{
	Game::master_renderer->Render();
}

GameManager::GameManager()
{
	int fakeargc = 1;
	char *fakeargv[] = { "fake", NULL };
	glutInit(&fakeargc, fakeargv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 320);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutCreateWindow("Game");

	glewInit();
	GLInit();
	GameInit();

	glutDisplayFunc(RenderScene);
}

GameManager::~GameManager()
{
	cout << endl  << "======" << endl << "Cleaning up.." << endl;
	delete model_loader;
	delete master_renderer;
}

void GameManager::GLInit()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
	
void GameManager::GameInit()
{
	model_loader = new ModelLoader();
	master_renderer = new MasterRenderer(this);
	Game::master_renderer = master_renderer;
}

void GameManager::MainLoop()
{
	glutMainLoop();
}
