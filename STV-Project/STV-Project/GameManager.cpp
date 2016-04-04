#include "GameManager.h"
#include "Model.h"
#include "Camera.h"
#include "Keyboard.h"
#include "Timer.h"

#include <vector>
#include <thread>


void RenderScene()
{
	GameManager::getMain()->VisualUpdate();
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
	cout << endl  << "======" << endl << "Cleaning up GameManager.." << endl;
	delete model_loader;
	delete master_renderer;
	delete texture_loader;
	delete voxel_builder;
	delete Camera::getMain();
	delete Keyboard::getMain();
	cout << "GameManager finished." << endl << "======" << endl;
}

void GameManager::GLInit()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
	
void GameManager::GameInit()
{
	texture_loader = new TextureLoader;
	model_loader = new ModelLoader;
	master_renderer = new MasterRenderer(this);
	voxel_builder = new MarchingCube;
}

void GameManager::MainLoop()
{
	running = true;
	thread logic_thread(&GameManager::LogicLoop, this);
	logic_thread.detach();
	VisualLoop();
	running = false;

	while (true) 
	{
		if (!logic_running)
			break;
	}
}

void GameManager::SetCurrentScene(Scene* scene)
{
	if (_current_scene != nullptr) _current_scene->DettachScene();
	master_renderer->ClearAllRenderQueues();

	_current_scene = scene;
	if(scene != nullptr) scene->AttachScene();
}



void GameManager::LogicLoop() 
{
	logic_running = true;
	cout << "Starting 'Logic Loop'\n";

	while (running)
	{
		update_timer.start();
		LogicUpdate();
		update_timer.HoldUntilExceeded(UPDATE_SLEEP);
	}

	cout << "Finshed 'Logic Loop'\n"; 
	logic_running = false;
}

void GameManager::LogicUpdate() 
{
	Camera::getMain()->TransformUpdate();
	GameManager::getMain()->GetCurrentScene()->LogicUpdate();
}



void GameManager::VisualLoop() 
{
	cout << "Starting 'Visual Loop'\n";
	glutMainLoop();
	cout << "Finshed 'Visual Loop'\n";
}

void GameManager::VisualUpdate() 
{
	LERP_time = GetDynamicLERPTime();

	GameManager::getMain()->GetCurrentScene()->VisualUpdate();
}