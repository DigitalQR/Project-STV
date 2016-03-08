#include "Entity.h"
#include "GameManager.h"
#include <iostream>

Entity::Entity()
{
}

Entity::~Entity()
{
	for (Component* component : _components)
		delete component;
	for (Element3D* element : _elements)
		delete element;
}

void Entity::Start() 
{
	for (Component* component : _components)
		component->Start();
}

void Entity::LogicUpdate()
{
	for (Component* component : _components)
		component->LogicUpdate();
};

void Entity::VisualUpdate()
{
	for (Component* component : _components)
		component->VisualUpdate();
};


void Entity::AddComponent(Component* component)
{
	_components.push_back(component);
	component->Attach(this);
}

void Entity::RemoveComponentAndDelete(Component* component)
{
	int index = -1; 

	for (int i = 0; i < _components.size(); i++)
	{
		if (_components[i] == component)
		{
			index = i;
			break;
		}
	}

	if (index != -1)
	{
		_components.erase(_components.begin() + index);
		delete component;
	}
}


void Entity::AddElement(Element3D* element)
{
	_elements.push_back(element);
	element->setParent(this);
}

void Entity::RemoveElementAndDelete(Element3D* element)
{
	int index = -1;

	for (int i = 0; i < _elements.size(); i++)
	{
		if (_elements[i] == element)
		{
			index = i;
			break;
		}
	}

	if (index != -1)
	{
		_elements.erase(_elements.begin() + index);
		delete element;
	}
}

void Entity::AddElementsForRender() 
{
	this->AddElementsForRender(GameManager::getMain()->master_renderer->static_shader);
}