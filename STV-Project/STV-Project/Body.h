#pragma once
#include "Component.h"
#include "Dependencies\glm\common.hpp"


using namespace glm;


class Body : public Component
{
public:
	vec3 velocity;

	void Start() {}

	void VisualUpdate() {}

	void LogicUpdate() {}

};