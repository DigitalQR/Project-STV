#pragma once
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Shader.h"

class StaticShader : public Shader
{
public:
	StaticShader();
	~StaticShader();

	void PrepareShader();
	void PrepareModel(Model* model);
	void PrepareInstance(Entity* entity);

private:
	const GLuint _UNIFORM_MODEL_MATRIX;
	const GLuint _UNIFORM_VIEW_MATRIX;
	const GLuint _UNIFORM_PROJECTION_MATRIX;
};

