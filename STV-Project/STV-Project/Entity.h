#pragma once
#include "Model.h"
#include "Transform.h"

class Entity
{
public:
	Model* model;
	Transform transform;
	Entity(Model* model);
	~Entity();
};

