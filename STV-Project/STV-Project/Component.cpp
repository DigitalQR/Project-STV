#include "Component.h"


Component::Component()
{
}


Component::~Component()
{
}

void Component::Attach(Transform* parent)
{
	this->parent = parent;
}
