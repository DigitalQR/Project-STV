#pragma once
#include "Dependencies\glm\glm.hpp"
#include "BlockGlossary.h"
#include <vector>
#include <iostream>

using namespace std;
using namespace glm;


struct Vectori {
	Vectori(int x, int y, int z) : x(x), y(y), z(z) {};
	int x, y, z;

	Vectori operator*(Vectori& b)
	{
		return Vectori(x*b.x, y*b.y, z*b.z);
	}

	bool operator==(Vectori& b)
	{
		return x == b.x && y == b.y && z == b.z;
	}

	bool operator!=(Vectori& b)
	{
		return x != b.x || y != b.y || z != b.z;
	}

	friend Vectori operator* (const Vectori& a, const Vectori& b)
	{
		return Vectori(a.x*b.x, a.y*b.y, a.z*b.z);
	}

	friend bool operator== (const Vectori& a, const Vectori& b)
	{
		return (a.x == b.x && a.y == b.y && a.z == b.z);
	}
};


struct ModelData
{
	vector<float> verts;
	vector<float> uvs;
	vector<float> normals;
	vector<unsigned int> indices;
	vector<unsigned int> texture_ids;

	ModelData(vector<float> verts, vector<float> uvs, vector<float> normals, vector<unsigned int> texture_ids, vector<unsigned int> indices) :
		verts(verts), uvs(uvs), normals(normals), indices(indices), texture_ids(texture_ids) {};

	ModelData() {};

	void Clear();

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

		texture_ids.clear();
		texture_ids.resize(0);
		texture_ids.insert(texture_ids.begin(), b.texture_ids.begin(), b.texture_ids.end());
	}

	void operator+=(ModelData& b)
	{
		int end_index = verts.size() / 3;
		verts.insert(verts.end(), b.verts.begin(), b.verts.end());
		normals.insert(normals.end(), b.normals.begin(), b.normals.end());
		uvs.insert(uvs.end(), b.uvs.begin(), b.uvs.end());
		texture_ids.insert(texture_ids.end(), b.texture_ids.begin(), b.texture_ids.end());

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

	void Smooth(Resource v0, Resource v1, Resource v2, Resource v3, Resource v4, Resource v5, Resource v6, Resource v7);

	void ReplaceTextureIDs(resource_id v0, resource_id v1, resource_id v2, resource_id v3, resource_id v4, resource_id v5, resource_id v6, resource_id v7);

	void BuildNormals();

	void BuildUVs();

	void Flip();

	bool isEmpty()
	{
		return verts.size();
	}

	void Rotate(unsigned int x, unsigned int y, unsigned int z, float pivot_x, float pivot_y, float pivot_z);
};