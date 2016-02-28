#pragma once
#include "MemoryListener.h"

#include "Dependencies\glew\glew.h"
#include "Model.h"

class TexturedModel
{

public:
	TexturedModel(Model* model, GLuint texture) :
		model(model),
		texture(texture)
	{};

	Model* model;
	GLuint texture;
};

