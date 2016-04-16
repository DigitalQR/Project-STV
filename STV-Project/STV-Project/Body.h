#pragma once
#include "Component.h"
#include "Dependencies\glm\common.hpp"


using namespace glm;


class Body : public Component
{
public:
	vec3 velocity;
	float gravity_scale = 1.0f;

	void Start() {}

	void VisualUpdate() {}

	void LogicUpdate() {}

};