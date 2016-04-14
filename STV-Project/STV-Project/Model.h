#pragma once
#include "MemoryListener.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include <vector>

using namespace std;


struct Model
{
public:
	Model(GLuint VAO, vector<GLuint> VBOs, unsigned int indices_size) :
		_vao(VAO), 
		_vbos(VBOs),
		_indices_size(indices_size) 
	{};
	~Model();

	GLuint polygon_mode = GL_FILL;
	GLuint getVAO() { return _vao; }
	vector<GLuint> getVBOs() { return _vbos; }
	unsigned int getIndiceCount() { return _indices_size;  }

private:
	GLuint _vao;
	vector<GLuint> _vbos;
	unsigned int _indices_size;
};

