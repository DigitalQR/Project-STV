#pragma once
#include "Body.h"
#include "Dependencies\glm\common.hpp"


using namespace glm;


class EllipsoidBody : public Body
{
private:
	vec3 _size;

public:

	EllipsoidBody(vec3 size)
	{
		SetSize(size);
	}

	EllipsoidBody()
	{
		_size = vec3(1, 1, 1);
	}

	void Start() {}

	void VisualUpdate() {}

	void LogicUpdate() {}

	void SetSize(vec3 size)
	{
		if (size.x < 0)
			size.x *= -1.0f;

		if (size.y < 0)
			size.y *= -1.0f;

		if (size.z < 0)
			size.z *= -1.0f;

		if (size.x == 0)
			size.x = 0.00001f;

		if (size.y == 0)
			size.y = 0.00001f;

		if (size.z == 0)
			size.z = 0.00001f;

		_size = size;
	}

	vec3 GetSize()
	{
		return _size;
	}

};