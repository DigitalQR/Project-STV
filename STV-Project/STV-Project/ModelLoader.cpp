#include "ModelLoader.h"
#include <iostream>
#include <vector>


Model* ModelLoader::CreateModel(vector<float> vertices, vector<float> uv_coords, vector<float> normals, vector<unsigned int> indices)
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	ModelLoader::_vaos.push_back(vao);
	vector<GLuint> vbos;

	glBindVertexArray(vao);
	BindIndicesBuffer(indices);
	vbos.push_back(StoreInVBO(0, 3, vertices, GL_STATIC_DRAW));
	vbos.push_back(StoreInVBO(1, 2, uv_coords, GL_STATIC_DRAW));
	vbos.push_back(StoreInVBO(2, 3, normals, GL_STATIC_DRAW));

	glBindVertexArray(0);
	return new Model(vao, vbos, indices.size());
}

Model* ModelLoader::CreateTerrainModel(vector<float> vertices, vector<float> uv_coords, vector<float> normals, vector<unsigned int> texture_ids, vector<unsigned int> indices)
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	ModelLoader::_vaos.push_back(vao);
	vector<GLuint> vbos;

	glBindVertexArray(vao);
	BindIndicesBuffer(indices);
	vbos.push_back(StoreInVBO(0, 3, vertices, GL_STATIC_DRAW));
	vbos.push_back(StoreInVBO(1, 2, uv_coords, GL_STATIC_DRAW));
	vbos.push_back(StoreInVBO(2, 3, normals, GL_STATIC_DRAW));
	vbos.push_back(StoreInVBO(3, 1, texture_ids, GL_STATIC_DRAW));

	glBindVertexArray(0);
	return new Model(vao, vbos, indices.size());
}

GLuint ModelLoader::StoreInVBO(GLuint attrib_number, int individual_data_size, vector<float>& data, const GLenum VBO_TYPE)
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	ModelLoader::_vbos.push_back(vbo);
	UpdateDataIn(vbo, attrib_number, individual_data_size, data, VBO_TYPE);
	return vbo;
}

void ModelLoader::UpdateDataIn(GLuint vbo, GLuint attrib_number, int individual_data_size, vector<float> &data, const GLenum VBO_TYPE)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], VBO_TYPE);

	//Attribute number in shader, number of data per vetex, data type, should the data be normalised, stride, offset
	glEnableVertexAttribArray(attrib_number);
	glVertexAttribPointer(attrib_number, individual_data_size, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint ModelLoader::StoreInVBO(GLuint attrib_number, int individual_data_size, vector<unsigned int>& data, const GLenum VBO_TYPE)
{
	vector<float> temp;
	temp.resize(data.size());
	for (int i = 0; i < data.size(); i++)
		temp[i] = data[i];

	return StoreInVBO(attrib_number, individual_data_size, temp, VBO_TYPE);
}

void ModelLoader::UpdateDataIn(GLuint vbo, GLuint attrib_number, int individual_data_size, vector<unsigned int> &data, const GLenum VBO_TYPE)
{
	vector<float> temp;
	temp.resize(data.size());
	for (int i = 0; i < data.size(); i++)
		temp[i] = data[i];

	UpdateDataIn(vbo, attrib_number, individual_data_size, temp, VBO_TYPE);
}

GLuint ModelLoader::BindIndicesBuffer(vector<unsigned int>& indices)
{
	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vbo;
}

void ModelLoader::DeleteModelData(Model* model) 
{
	const GLuint vao = model->getVAO();
	vector<GLuint>& vbos = model->getVBOs();
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(vbos.size(), &vbos[0]);

	for (int i = 0; i < _vaos.size(); i++)
		if (_vaos[i] == vao)
		{
			_vaos.erase(_vaos.begin() + i);
			break;
		}

	for (GLuint vbo : vbos)
		for (int i = 0; i < _vbos.size(); i++)
			if (_vbos[i] == vbo) 
			{
				_vbos.erase(_vbos.begin() + i);
				break;
			}
	
}

ModelLoader::~ModelLoader()
{
	if (_vaos.size() != 0)glDeleteVertexArrays(ModelLoader::_vaos.size(), &ModelLoader::_vaos[0]);
	if (_vbos.size() != 0)glDeleteBuffers(ModelLoader::_vbos.size(), &ModelLoader::_vbos[0]);
	std::cout << "Deleting all VAOs and VBOs." << endl;
}