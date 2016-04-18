#pragma once
#include "MemoryListener.h"

#include "Transform.h"
#include "TexturedModel.h"

class Element3D : public Transform
{
public:
	bool enabled;
	Element3D(TexturedModel* textured_model) : textured_model(textured_model) 
	{
		enabled = true;
	};
	~Element3D() {};
	TexturedModel* textured_model;
};

