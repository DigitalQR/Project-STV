#pragma once
#include "MemoryListener.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Shader.h"

class StaticShader : public Shader
{
public:
	StaticShader();
	~StaticShader();

	void PrepareShader();
	void PrepareModel(TexturedModel* textured_model);
	void PrepareInstance(Element3D* element);

private:
	const GLuint _UNIFORM_MODEL_MATRIX;
	const GLuint _UNIFORM_VIEW_MATRIX;
	const GLuint _UNIFORM_PROJECTION_MATRIX;
};

