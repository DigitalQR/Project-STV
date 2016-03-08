#pragma once
#include "MemoryListener.h"
#include <vector>
#include <typeinfo>
#include "Transform.h"
#include "Component.h"

#include "Element3D.h"
#include "Shader.h"

using namespace std;

/**
Entity:
-Holds models to be rendererd and is used logic side.
-Components attached to an entity will be ran at the begining, every update and every frame.


*/
class Entity : public Transform
{
public:
	Entity();
	virtual ~Entity();
	
	virtual void Start();
	virtual void LogicUpdate();
	virtual void VisualUpdate();
	
	/**
	Tries to find a component attached to Entity


	@params Template takes class of desired component
	@returns Component of passed class, if it is found
	*/
	template<class component_type>
	component_type* GetComponent()
	{
		for (Component* component : _components)
		{
			component_type* comp = dynamic_cast<component_type*>(component);
			if (comp != nullptr)
			{
				return comp;
			}
		}
		return nullptr;
	}

	void AddComponent(Component* component);
	void RemoveComponentAndDelete(Component* component);


	vector<Element3D*> GetElements() const { return _elements; }
	void AddElement(Element3D* element);
	void RemoveElementAndDelete(Element3D* component);

	virtual void AddElementsForRender();

protected:	
	/**
	Add Elements for render in desired shader
	
	@params Desired shader
	*/	
	void AddElementsForRender(Shader* shader)
	{
		for (Element3D* element : _elements)
		{
			AddElementForRender(element, shader);
		}
	}

	inline void AddElementForRender(Element3D* element, Shader* shader)
	{
		shader->AddForRender(element);
	}


private:
	vector<Component*> _components;
	vector<Element3D*> _elements;
};

