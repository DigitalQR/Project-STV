#pragma once
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\common.hpp"
#include <vector>


using namespace std;
using namespace glm;


struct Plane 
{
private:
	vec3 _normal;
	float _plane_constant;
	vec3 _origin;

public:

	Plane(vec3 origin, vec3 normal) : _origin(origin), _normal(normal)
	{
		_plane_constant = -(normal.x * origin.x + normal.y * origin.y + normal.z * origin.z);
	};

	vec3 GetOrigin() 
	{
		return _origin;
	}

	vec3 GetNormal() 
	{
		return _normal;
	}

	float SignedDistance(vec3 point)
	{
		return dot(point, _normal) + _plane_constant;
	}

};


struct Triangle 
{
private:
	vec3 _normal;
	float _plane_constant;

	void init() 
	{
		_normal = normalize(cross(b - a, c - a));
		_plane_constant = -(_normal.x * a.x + _normal.y * a.y + _normal.z * a.z);
	}

public:
	vec3 a;
	vec3 b;
	vec3 c;

	Triangle(const Triangle& tri) : a(tri.a), b(tri.b), c(tri.c)
	{
		init();
	}

	Triangle(vec3 a, vec3 b, vec3 c) : a(a), b(b), c(c)
	{
		init();
	};
		
	void operator=(const Triangle& tri)
	{
		a = tri.a;
		b = tri.b;
		c = tri.c;
		init();
	}

	vec3 GetNormal() 
	{
		return _normal;
	}

	bool IsFrontFacingTo(vec3 direction) 
	{
		float d = dot(_normal, direction);
		return (d <= 0);
	}

	float SignedDistanceToTrisPlane(vec3 point) 
	{
		return dot(point, _normal) + _plane_constant;
	}

	#define in(a) ((uint32&)a)
	bool IsPointInside(vec3 point) 
	{
		if (_normal.x * point.x + _normal.y * point.y + _normal.z * point.z != -_plane_constant)
			return false;

		Triangle& tri = *this;

		vec3 e10 = tri.b - tri.a;
		vec3 e20 = tri.c - tri.a;
		float a = dot(e10, e10);
		float b = dot(e10, e20);
		float c = dot(e20, e20);
		float ac_bb = (a*c) - (b*b);
		vec3 vp(point.x - tri.a.x, point.y - tri.a.y, point.z - tri.a.z);
		float d = dot(vp, e10);
		float e = dot(vp, e20);
		float x = (d*c) - (e*b);
		float y = (e*a) - (d*b);
		float z = x + y - ac_bb;
		return ((in(z)& ~(in(x) | in(y))) & 0x80000000);
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

	void operator=(TriMesh& b)
	{
		_tris = b._tris;
	}

	friend TriMesh& operator+(TriMesh& a, TriMesh& b) 
	{
		TriMesh c;
		c._tris.insert(c._tris.end(), a._tris.begin(), a._tris.end());
		c._tris.insert(c._tris.end(), b._tris.begin(), b._tris.end());
		return c;
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