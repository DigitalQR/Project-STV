#include "PhysicsEngine.h"



PhysicsEngine::PhysicsEngine()
{
}


PhysicsEngine::~PhysicsEngine()
{
}


void PhysicsEngine::PhysicsStep() 
{
	for (EllipsoidBody* body : _ellipsoid_bodies)
		Step(*body);
}

void PhysicsEngine::Step(EllipsoidBody& body) 
{
	if (body.velocity.x == 0 && body.velocity.y == 0 && body.velocity.z == 0)
		return;

	body.parent->location += body.velocity;
	body.velocity *= 0.8f;
}

void PhysicsEngine::SetScene(Scene* scene) 
{
	_current_scene = scene;
	_ellipsoid_bodies.clear();

	for (Entity* e : scene->GetEntities()) 
	{
		EllipsoidBody* body = e->GetComponent<EllipsoidBody>();
		if (body != nullptr)
			_ellipsoid_bodies.push_back(body);
	}
}