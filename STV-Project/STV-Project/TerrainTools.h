#pragma once
#include "Dependencies\glm\glm.hpp"
#include <vector>


using namespace std;
using namespace glm;


struct Vectori {
	Vectori(int x, int y, int z) : x(x), y(y), z(z) {};
	int x, y, z;

	Vectori operator*(Vectori& b)
	{
		return Vectori(x*b.x, y*b.y, z*b.z);
	}

	friend Vectori operator* (const Vectori& a, const Vectori& b)
	{
		return Vectori(a.x*b.x, a.y*b.y, a.z*b.z);
	}
};


struct ModelData
{
	vector<float> verts;
	vector<float> uvs;
	vector<float> normals;
	vector<unsigned int> indices;

	ModelData(vector<float> verts, vector<float> uvs, vector<float> normals, vector<unsigned int> indices) :
		verts(verts), uvs(uvs), normals(normals), indices(indices) {};

	ModelData() {};


	void operator=(ModelData& b)
	{
		verts.clear();
		verts.resize(0);
		verts.insert(verts.begin(), b.verts.begin(), b.verts.end());

		uvs.clear();
		uvs.resize(0);
		uvs.insert(uvs.begin(), b.uvs.begin(), b.uvs.end());

		normals.clear();
		normals.resize(0);
		normals.insert(normals.begin(), b.normals.begin(), b.normals.end());

		indices.clear();
		indices.resize(0);
		indices.insert(indices.begin(), b.indices.begin(), b.indices.end());
	}

	void operator+=(ModelData& b)
	{
		int end_index = verts.size() / 3;
		verts.insert(verts.end(), b.verts.begin(), b.verts.end());
		normals.insert(normals.end(), b.normals.begin(), b.normals.end());
		uvs.insert(uvs.end(), b.uvs.begin(), b.uvs.end());

		for (unsigned int index : b.indices)
			indices.push_back(index + end_index);
	}

	void operator+=(Vectori& b)
	{
		for (int i = 0; i < verts.size(); i += 3)
		{
			verts[i] += b.x;
			verts[i + 1] += b.y;
			verts[i + 2] += b.z;
		}
	}

	void BuildNormals() 
	{
		//Reset normals
		normals.clear();
		normals.resize(verts.size(), 0);
		
		
		//Work out all normals for faces
		for (unsigned int i = 0; i < indices.size(); )
		{
			unsigned int index0 = indices[i++] * 3;
			unsigned int index1 = indices[i++] * 3;
			unsigned int index2 = indices[i++] * 3;

			vec3 vert0(verts[index0], verts[index0 + 1], verts[index0 + 2]);
			vec3 vert1(verts[index1], verts[index1 + 1], verts[index1 + 2]);
			vec3 vert2(verts[index2], verts[index2 + 1], verts[index2 + 2]);

			vec3 norm = glm::cross(vert2 - vert1 , vert0 - vert1);

			normals[index0] += norm.x;
			normals[index1] += norm.x;
			normals[index2] += norm.x;

			normals[index0 + 1] += norm.x;
			normals[index1 + 1] += norm.y;
			normals[index2 + 1] += norm.y;

			normals[index0 + 2] += norm.z;
			normals[index1 + 2] += norm.z;
			normals[index2 + 2] += norm.z;

		}

		//Normalize
		for (unsigned int i = 0; i < normals.size(); i += 3)
		{
			if (normals[i] != 0 && normals[i+1] != 0 && normals[i+2] != 0)
			{
				//vec3 norm = glm::normalize(vec3(normals[i], normals[i + 1], normals[i + 2]));
				//normals[i] = norm.x;
				//normals[i + 1] = norm.y;
				//normals[i + 2] = norm.z;
			}
			
		}	
		
	}

	void Flip()
	{
		reverse(indices.begin(), indices.end());
	}

	bool isEmpty()
	{
		return verts.size();
	}

	void Mirror(bool x, bool y, bool z, float pivot_x, float pivot_y, float pivot_z) 
	{
		for (unsigned int i = 0; i < verts.size(); i += 3) {
			verts[i] -= pivot_x;
			verts[i + 1] -= pivot_y;
			verts[i + 2] -= pivot_z;
		}

		for (unsigned int i = 0; i < verts.size(); i += 3) 
		{
			if (x) verts[i] *= -1;
			if (y) verts[i+1] *= -1;
			if (z) verts[i+2] *= -1;
		}

		for (unsigned int i = 0; i < verts.size(); i += 3) {
			verts[i] += pivot_x;
			verts[i + 1] += pivot_y;
			verts[i + 2] += pivot_z;
		}

		if (x)
			Flip();
		if (y)
			Flip();
		if (z)
			Flip();
	}

	void Rotate(unsigned int x, unsigned int y, unsigned int z, float pivot_x, float pivot_y, float pivot_z)
	{
		for (unsigned int i = 0; i < verts.size(); i += 3) {
			verts[i] -= pivot_x;
			verts[i + 1] -= pivot_y;
			verts[i + 2] -= pivot_z;
		}

		float temp_x;
		float temp_y;
		float temp_z;

		for (unsigned int i = 0; i < verts.size(); i += 3) {
			for (unsigned int t = x; t != 0; t--)
			{
				temp_x = verts[i];
				temp_y = verts[i + 1];
				temp_z = verts[i + 2];

				verts[i] = temp_x;
				verts[i + 1] = -temp_z;
				verts[i + 2] = temp_y;
			}
			for (unsigned int t = y; t != 0; t--)
			{
				temp_x = verts[i];
				temp_y = verts[i + 1];
				temp_z = verts[i + 2];

				verts[i] = temp_z;
				verts[i + 1] = temp_y;
				verts[i + 2] = -temp_x;
			}
			for (unsigned int t = z; t != 0; t--)
			{
				temp_x = verts[i];
				temp_y = verts[i + 1];
				temp_z = verts[i + 2];

				verts[i] = temp_y;
				verts[i + 1] = -temp_x;
				verts[i + 2] = temp_z;
			}
		}

		for (unsigned int i = 0; i < verts.size(); i += 3) {
			verts[i] += pivot_x;
			verts[i + 1] += pivot_y;
			verts[i + 2] += pivot_z;
		}
	}
};