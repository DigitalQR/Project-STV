#pragma once
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"

class Model
{
public:
	Model(GLuint VAO) : _VAO(VAO) {};
	GLuint getVAO() { return _VAO; }

private:
	GLuint _VAO;
};

