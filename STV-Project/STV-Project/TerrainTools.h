#pragma once


#include <vector>

using namespace std;

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
		verts.resize(b.verts.size());
		verts.insert(verts.begin(), b.verts.begin(), b.verts.end());

		uvs.clear();
		uvs.resize(b.uvs.size());
		uvs.insert(uvs.begin(), b.uvs.begin(), b.uvs.end());

		normals.clear();
		normals.resize(b.normals.size());
		normals.insert(normals.begin(), b.normals.begin(), b.normals.end());

		indices.clear();
		indices.resize(b.indices.size());
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

	void Flip()
	{
		reverse(indices.begin(), indices.end());
		for (float& f : normals)
			f *= -1;
	}

	bool isEmpty()
	{
		return verts.size();
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
		float temp_xn;
		float temp_yn;
		float temp_zn;

		for (unsigned int i = 0; i < verts.size(); i += 3) {
			for (unsigned int t = x; t != 0; t--)
			{
				temp_x = verts[i];
				temp_y = verts[i + 1];
				temp_z = verts[i + 2];
				temp_xn = normals[i];
				temp_yn = normals[i + 1];
				temp_zn = normals[i + 2];

				verts[i] = temp_x;
				verts[i + 1] = -temp_z;
				verts[i + 2] = temp_y;
				normals[i] = temp_xn;
				normals[i + 1] = -temp_zn;
				normals[i + 2] = temp_yn;
			}
			for (unsigned int t = y; t != 0; t--)
			{
				temp_x = verts[i];
				temp_y = verts[i + 1];
				temp_z = verts[i + 2];
				temp_xn = normals[i];
				temp_yn = normals[i + 1];
				temp_zn = normals[i + 2];

				verts[i] = temp_z;
				verts[i + 1] = temp_y;
				verts[i + 2] = -temp_x;
				normals[i] = temp_zn;
				normals[i + 1] = temp_yn;
				normals[i + 2] = -temp_xn;
			}
			for (unsigned int t = z; t != 0; t--)
			{
				temp_x = verts[i];
				temp_y = verts[i + 1];
				temp_z = verts[i + 2];
				temp_xn = normals[i];
				temp_yn = normals[i + 1];
				temp_zn = normals[i + 2];

				verts[i] = temp_y;
				verts[i + 1] = -temp_x;
				verts[i + 2] = temp_z;
				normals[i] = temp_yn;
				normals[i + 1] = -temp_xn;
				normals[i + 2] = temp_zn;
			}
		}

		for (unsigned int i = 0; i < verts.size(); i += 3) {
			verts[i] += pivot_x;
			verts[i + 1] += pivot_y;
			verts[i + 2] += pivot_z;
		}
	}
};