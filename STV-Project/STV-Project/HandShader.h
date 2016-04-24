#pragma once
#include "MemoryListener.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Shader.h"

class HandShader : public Shader
{
public:
	HandShader();
	~HandShader();

	void PrepareShader();
	bool PrepareModel(TexturedModel* textured_model);
	void PrepareInstance(Element3D* element);

private:
	const GLuint _UNIFORM_MODEL_MATRIX;
	const GLuint _UNIFORM_VIEW_MATRIX;
	const GLuint _UNIFORM_PROJECTION_MATRIX;

	const GLuint _UNIFORM_SUNLIGHT_COLOUR;
	const GLuint _UNIFORM_SUNLIGHT_DIRECTION;

	const GLuint _UNIFORM_LIGHT_COLOUR;

	const GLuint _UNIFORM_ITEM_ID;
};

