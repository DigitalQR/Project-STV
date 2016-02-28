#pragma once
#include "MemoryListener.h"
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Model.h"

#include <vector>

using namespace std;

class ModelLoader
{
public:
	~ModelLoader();

	/**
	Generates a VAO and stores the data in the correct attribute 
	position and return a Model with the VAO attached

	Attribute Positions:
	0 = Vertices
	1 = UVs

	@params Vector holding the vertices in format x0,y0,z0,x1,y2,z2 etc.
	@return Model
	*/
	Model* CreateModel(vector<float> vertices, vector<float> uv_coords, vector<unsigned int> indices);

	/**
	Generates a VBO and stores the data in the given attribute position

	@params Attribute position, Data group size i.e. 3D vector = 3, 2D vector = 2; Raw data to be stored, type of access.
	@return VBO ID
	*/
	GLuint StoreInVBO(GLuint attrib_number, int individual_data_size, vector<float> &data, const GLenum VBO_TYPE);

private:
	vector<GLuint> _vaos;
	vector<GLuint> _vbos;

	/**
	Generates a VBO and stores the indices data in int

	@params Vector of indices
	@return VBO ID for index buffer
	*/
	GLuint BindIndicesBuffer(vector<unsigned int>& indices);
};

