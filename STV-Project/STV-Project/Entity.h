#pragma once
#include "MemoryListener.h"
#include <vector>
#include "Model.h"
#include "Transform.h"
#include "Component.h"

using namespace std;

class Entity : public Transform
{
public:
	Entity(Model* model);
	~Entity();

	Model* model;

	virtual void LogicUpdate();
	virtual void VisualUpdate();

protected:
	void AddComponent(Component* component);
	void RemoveComponentAndDelete(Component* component);

private:
	vector<Component*> _components;
};

