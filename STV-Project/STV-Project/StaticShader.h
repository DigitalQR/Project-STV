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

private:
	GLuint _UNIFORM_VIEW_MATRIX;
	GLuint _UNIFORM_PROJECTION_MATRIX;
};

