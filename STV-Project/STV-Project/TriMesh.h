#pragma once
#include "Dependencies\glm\glm.hpp"
#include <vector>

using namespace glm;
using namespace std;

struct Triangle 
{
public:
	vec3 a;
	vec3 b;
	vec3 c;

	Triangle(vec3 a, vec3 b, vec3 c) : a(a), b(b), c(c) {};

	vec3 GetNormal() 
	{
		return cross(b - a, c - a);
	}
};

struct TriMesh 
{

public:

	TriMesh()
	{
	}

	TriMesh(vector<float>& verts, vector<unsigned int>& indices) 
	{
		ConvertToTriangles(verts, indices);
	}

	vector<Triangle>& GetTriangles()
	{
		return _tris;
	}

	void ConvertToTriangles(vector<float>& verts, vector<unsigned int>& indices) 
	{
		_tris.clear();
		_tris.reserve(indices.size() / 3);
		for (int i = 0; i < indices.size(); i += 3) 
		{
			vec3 a(verts[indices[i] * 3], verts[indices[i] * 3 + 1], verts[indices[i] * 3 + 2]);
			vec3 b(verts[indices[i + 1] * 3], verts[indices[i + 1] * 3 + 1], verts[indices[i + 1] * 3 + 2]);
			vec3 c(verts[indices[i + 2] * 3], verts[indices[i + 2] * 3 + 1], verts[indices[i + 2] * 3 + 2]);
			_tris.push_back(Triangle(a,b,c));
		}
	}

private:
	vector<Triangle> _tris;
};