#pragma once
#include "MemoryListener.h"
#include <vector>
#include <typeinfo>
#include "Transform.h"
#include "Component.h"

#include "Element3D.h"

using namespace std;

class Entity : public Transform
{
public:
	Entity();
	~Entity();
	
	virtual void LogicUpdate();
	virtual void VisualUpdate();

	void AddComponent(Component* component);
	void RemoveComponentAndDelete(Component* component);

	vector<Element3D*> GetElements() const { return _elements; }

	void AddElement(Element3D* element);
	void RemoveElementAndDelete(Element3D* component);

private:
	vector<Component*> _components;
	vector<Element3D*> _elements;
};

