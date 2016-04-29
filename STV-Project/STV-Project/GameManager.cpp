#include "GameManager.h"
#include "Model.h"
#include "Camera.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Timer.h"

#include <vector>
#include <thread>


void RenderScene()
{
	Timer timer;
	timer.start();

	Camera::getMain()->frustum.ClearTests();
	GameManager::getMain()->VisualUpdate();
	GameManager::getMain()->master_renderer->Render();

	timer.HoldUntilExceeded(GameManager::getMain()->GetMaxFPS());
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

void MouseClick(int button, int state, int x, int y) 
{
	switch (button) 
	{
	case GLUT_LEFT_BUTTON:
		Mouse::SetState(0, state == GLUT_DOWN);
		break;
	case GLUT_RIGHT_BUTTON:
		Mouse::SetState(1, state == GLUT_DOWN);
		break;
	case GLUT_MIDDLE_BUTTON:
		Mouse::SetState(2, state == GLUT_DOWN);
		break;
	};
}

void MouseMotion(int x, int y) 
{
	Mouse::UpdateMouseInfo(x, y);
}

void MouseWheel(int button, int direction, int x, int y)
{
	Mouse::SetScrollAmount(direction);
}

GameManager::GameManager()
{
	int fakeargc = 1;
	char *fakeargv[] = { "fake", NULL };
	glutInit(&fakeargc, fakeargv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutCreateWindow("Game");

	glewInit();
	GLInit();
	GameInit();

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ReshapeWindow);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutMouseFunc(MouseClick);
	glutMouseWheelFunc(MouseWheel);
	glutPassiveMotionFunc(MouseMotion);
}

GameManager::~GameManager()
{
	cout << endl  << "======" << endl << "Cleaning up GameManager.." << endl;
	delete model_loader;
	delete master_renderer;
	delete texture_loader;
	delete voxel_builder;
	delete physics_engine;
	delete Camera::getMain();
	delete Keyboard::getMain();
	delete Mouse::getMain();
	cout << "GameManager finished." << endl << "======" << endl;
}

void GameManager::GLInit()
{
	sky_colour = vec3(135.0f / 255.0f, 206.0f / 255.0f, 250.0f / 255.0f);
	sunlight_colour = vec3(1,1,1);
	sunlight_direction = vec3(0,-1, 0);

	fog_colour = vec3(224.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
	fog_density = 0.019f;
	fog_gradient = 6;

	player_light = Light(vec3(0, 0, 0), vec3(1, 0.6f, 0.2f), vec3(1, 0.01f, 0.02f));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
	
void GameManager::GameInit()
{
	physics_engine = new PhysicsEngine;
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
		{
			break;
		}
		else 
		{
			cout << "";
		}
	}
}

void GameManager::SetCurrentScene(Scene* scene)
{
	if (_current_scene != nullptr) _current_scene->DettachScene();
	master_renderer->ClearAllRenderQueues();

	_current_scene = scene;
	if (scene != nullptr)
	{
		scene->AttachScene();
		physics_engine->SetScene(scene);
	}
}



void GameManager::LogicLoop() 
{
	logic_running = true;
	cout << "Starting 'Logic Loop'\n";

	float overtime = 0;

	while (running)
	{
		update_timer.start();
		LogicUpdate();
		physics_engine->PhysicsStep();


		if (update_timer.HasExceeded(UPDATE_SLEEP))
		{
			overtime += update_timer.GetElapsedTimef() - UPDATE_SLEEP;
		}
		else
		{
			update_timer.HoldUntilExceeded(UPDATE_SLEEP - overtime);

			if (overtime >= UPDATE_SLEEP)
				overtime -= UPDATE_SLEEP;
			else
				overtime = 0;

		}
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
	GameManager::getMain()->GetCurrentScene()->VisualUpdate();
	LERP_time = GetDynamicLERPTime();
}