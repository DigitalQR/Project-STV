#include "GameManager.h"
#include "Model.h"
#include "Camera.h"
#include "Keyboard.h"
#include <vector>


void RenderScene()
{
	GameManager::getMain()->master_renderer->Render();
}

void ReshapeWindow(int width, int height)
{
	if (height == 0)
		height = 1;

	float aspect_ratio = width * 1.0f / height;
	Camera::getMain()->rebuildProjectionMatrix(aspect_ratio);

	glViewport(0, 0, width, height);
}

void KeyDown(unsigned char key, int x, int y)
{
	Keyboard::setState(key, true);
}

void KeyUp(unsigned char key, int x, int y)
{
	Keyboard::setState(key, false);
}

GameManager::GameManager()
{
	int fakeargc = 1;
	char *fakeargv[] = { "fake", NULL };
	glutInit(&fakeargc, fakeargv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1280, 720);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutCreateWindow("Game");

	glewInit();
	GLInit();
	GameInit();

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ReshapeWindow);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
}

GameManager::~GameManager()
{
	cout << endl  << "======" << endl << "Cleaning up.." << endl;
	delete model_loader;
	delete master_renderer;
	delete Camera::getMain();
	delete Keyboard::getMain();
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
}

void GameManager::MainLoop()
{
	cout << "Starting 'glutMainLoop'" << endl;
	glutMainLoop();
	cout << "Finshed 'glutMainLoop'" << endl;
}
