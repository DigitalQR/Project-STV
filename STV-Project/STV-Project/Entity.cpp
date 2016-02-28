#include "Entity.h"


Entity::Entity(TexturedModel* textured_model) : textured_model(textured_model)
{
}


Entity::~Entity()
{
	for (Component* component : _components)
		delete component;
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
