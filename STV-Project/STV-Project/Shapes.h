#pragma once
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\common.hpp"


using namespace glm;


struct Plane
{
private:
	vec3 _origin;
	vec3 _normal;
	float _plane_constant;

public:

	Plane() {}

	Plane(vec3 origin, vec3 normal) : _origin(origin), _normal(normal),
		_plane_constant(-(normal.x * origin.x + normal.y * origin.y + normal.z * origin.z)) {}

	//Where a,b and c are clockwise
	Plane(vec3 triangle_point_a, vec3 triangle_point_b, vec3 triangle_point_c) : _origin(triangle_point_a)
	{
		_normal = normalize(cross(triangle_point_b - triangle_point_a, triangle_point_c - triangle_point_a));
		_plane_constant = -(_normal.x * _origin.x + _normal.y * _origin.y + _normal.z * _origin.z);
	}

	void operator=(Plane& b) 
	{
		_origin = b._origin;
		_normal = b._normal;
		_plane_constant = b._plane_constant;
	}

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


struct Box 
{
private:
	const vec3 _centre;
	const vec3 _size;

public:
	Box() {};
	Box(vec3 centre, vec3 size) : _centre(centre), _size(size) {};
	Box(vec3 bottom_left, vec3 size, bool dud) : _centre(bottom_left + size/ 2.0f), _size(size) {};

	vec3 GetCentre() 
	{
		return _centre;
	}

	vec3 GetSize() 
	{
		return _size;
	}

	vec3 GetPoint(unsigned int index) 
	{
		switch (index)
		{
		case 0:
			return _centre + vec3(-_size.x, -_size.y, -_size.z) / 2.0f;
			break;
		case 1:
			return _centre + vec3(_size.x, -_size.y, -_size.z) / 2.0f;
			break;
		case 2:
			return _centre + vec3(_size.x, -_size.y, _size.z) / 2.0f;
			break;
		case 3:
			return _centre + vec3(-_size.x, -_size.y, _size.z) / 2.0f;
			break;
			
		case 4:
			return _centre + vec3(-_size.x, _size.y, -_size.z) / 2.0f;
			break;
		case 5:
			return _centre + vec3(_size.x, _size.y, -_size.z) / 2.0f;
			break;
		case 6:
			return _centre + vec3(_size.x, _size.y, _size.z) / 2.0f;
			break;
		case 7:
			return _centre + vec3(-_size.x, _size.y, _size.z) / 2.0f;
			break;

		default:
			return _centre;
			break;
		}
	}

};