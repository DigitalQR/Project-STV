#pragma once
#include "MemoryListener.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"


using namespace std;


struct Model
{
public:
	Model(GLuint VAO, unsigned int indices_size) : 
		_vao(VAO), 
		_indices_size(indices_size) 
	{};
	~Model();

	GLuint polygon_mode = GL_FILL;
	GLuint getVAO() { return _vao; }
	unsigned int getIndiceCount() { return _indices_size;  }

private:
	GLuint _vao;
	unsigned int _indices_size;
};

