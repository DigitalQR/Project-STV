#include "ModelLoader.h"
#include <iostream>


Model* ModelLoader::CreateModel(vector<float> vertices)
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	ModelLoader::_vaos.push_back(vao);

	glBindVertexArray(vao);
	StoreInVBO(0, 3, vertices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	return new Model(vao);
}

GLuint ModelLoader::StoreInVBO(GLuint attrib_number, int individual_data_size, vector<float>& data, const GLenum VBO_TYPE)
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	ModelLoader::_vbos.push_back(vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * 4, &data[0], VBO_TYPE);

	//Attribute number in shader, number of data per vetex, data type, should the data be normalised, stride, offset
	glEnableVertexAttribArray(attrib_number);
	glVertexAttribPointer(attrib_number, individual_data_size, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vbo;
}

ModelLoader::~ModelLoader()
{
	glDeleteVertexArrays(ModelLoader::_vaos.size(), &ModelLoader::_vaos[0]);
	glDeleteBuffers(ModelLoader::_vbos.size(), &ModelLoader::_vbos[0]);
	std::cout << "Deleting all VAOs and VBOs." << endl;
}