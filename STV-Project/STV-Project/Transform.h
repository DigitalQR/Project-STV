#pragma once
#include "MemoryListener.h"
#include "Dependencies\glm\common.hpp"
#include "Dependencies\glm\mat4x4.hpp"

using namespace glm;

class Transform
{
public:
	Transform();
	Transform(vec3& location);
	Transform(vec3& location, vec3& rotation);
	Transform(vec3& location, vec3& rotation, float scale);
	~Transform();

	vec3 getLERPLocation();
	mat4& getModelMatrix();
	vec3 location;
	vec3 rotation;
	float scale;

private:
	mat4 _model_matrix;

	vec3 _last_location;
	vec3 _last_rotation;
	float _last_scale;

	void Init(vec3& location, vec3& rotation, float scale);
};

