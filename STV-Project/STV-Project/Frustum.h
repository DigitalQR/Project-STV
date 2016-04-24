#pragma once
#include "Element3D.h"
#include "Shapes.h"
#include "Chunk.h"
#include <vector>
#include <array>

using namespace std;


struct element_pair 
{
public:
	Element3D* element;
	Box aabb;

	element_pair() 
	{
		element = nullptr;
	}

	element_pair(Element3D* element, Box aabb) : element(element), aabb(aabb) {};
};


class Camera;


class Frustum 
{
private:
	Camera& _camera;
	vector<element_pair> _tests;
	array<Plane, 6> _planes;

public:
	Frustum(Camera& camera) : _camera(camera) {};

	void AddToTest(Chunk* chunk)
	{
		if (chunk->GetEmptyModelFlag())
			return;

		_tests.push_back(

			element_pair(chunk->GetElement(),
				Box(vec3(chunk->MESH_OFFSET.x * chunk->MESH_SIZE.x, chunk->MESH_OFFSET.y * chunk->MESH_SIZE.y, chunk->MESH_OFFSET.z * chunk->MESH_SIZE.z),
					vec3(chunk->MESH_SIZE.x, chunk->MESH_SIZE.y, chunk->MESH_SIZE.z), true
					)
				)
		
			);
	}
	
	void ClearTests()	
	{
		_tests.clear();
	}

	void GenerateFrustum();

	void RunTests();

};