#pragma once
#include <vector>
#include "Scene.h"
#include "Body.h"
#include "EllipsoidBody.h"


using namespace std;


class PhysicsEngine
{
public:
	PhysicsEngine();
	~PhysicsEngine();

	void SetScene(Scene* scene);

	void PhysicsStep();

private:
	vector<EllipsoidBody*> _ellipsoid_bodies;
	Scene* _current_scene;

	void Step(EllipsoidBody& body);
};

