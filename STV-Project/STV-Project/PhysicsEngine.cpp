#include "PhysicsEngine.h"
#include "Chunk.h"
#include "EllipsoidSolver.h"

PhysicsEngine::PhysicsEngine()
{
}


PhysicsEngine::~PhysicsEngine()
{
}


void PhysicsEngine::PhysicsStep() 
{
	for (EllipsoidBody* body : _ellipsoid_bodies)
		if (body->enabled) 
		{
			body->velocity += _current_scene->gravity * body->gravity_scale;
			Step(*body);
			body->parent->location += body->velocity;
			body->velocity *= 0.8f;
		}
}

void PhysicsEngine::Step(EllipsoidBody& body) 
{
	if (body.velocity.x == 0 && body.velocity.y == 0 && body.velocity.z == 0)
		return;

	//Vectori start_chunk = Chunk::GetChunkCoordsOf(body.parent->location);
	//Vectori end_chunk = Chunk::GetChunkCoordsOf(body.parent->location + body.velocity);
	//ChunkLoader& chunk_data = *_current_scene->GetTerrain()->GetChunkLoader();

	//TriMesh terrain_mesh = chunk_data.GetChunk(start_chunk.x, start_chunk.y, start_chunk.z)->GetMesh();
	//TO-DO add adjacent chunk checks

	TriMesh tri;
	vector<float> verts{ 0,30,0, 0,30,50, 50,30,0 };
	vector<unsigned int> inds{ 0,1,2 };

	tri.ConvertToTriangles(verts, inds);
	EllipsoidSolver::ResolvedCollisions(body, tri);
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