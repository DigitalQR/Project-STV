#pragma once
#include "Dependencies\glm\glm.hpp"
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
	vector<unsigned int> texture_ids;

	ModelData(vector<float> verts, vector<float> uvs, vector<float> normals, vector<unsigned int> texture_ids, vector<unsigned int> indices) :
		verts(verts), uvs(uvs), normals(normals), indices(indices), texture_ids(texture_ids) {};

	ModelData() {};

	void Clear() 
	{
		verts.clear();
		uvs.clear();
		normals.clear();
		indices.clear();
		texture_ids.clear();
	}

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

	void ReplaceTextureIDs(resource_id v0, resource_id v1, resource_id v2, resource_id v3, resource_id v4, resource_id v5, resource_id v6, resource_id v7) 
	{
		for (unsigned int& id : texture_ids) 
		{
			switch (id) 
			{
			case 0:
				id = (unsigned int)v0;
				break;
			case 1:
				id = (unsigned int)v1;
				break;
			case 2:
				id = (unsigned int)v2;
				break;
			case 3:
				id = (unsigned int)v3;
				break;
			case 4:
				id = (unsigned int)v4;
				break;
			case 5:
				id = (unsigned int)v5;
				break;
			case 6:
				id = (unsigned int)v6;
				break;
			case 7:
				id = (unsigned int)v7;
				break;
			};
		}
	}

	void BuildNormals() 
	{
		//Reset normals
		normals.clear();
		normals.resize(verts.size(), 0);
		
		//Work out all normals for faces
		for (unsigned int i = 0; i < indices.size(); i+=3)
		{
			unsigned int i0 = indices[i] * 3;
			unsigned int i1 = indices[i + 1] * 3;
			unsigned int i2 = indices[i + 2] * 3;

			vec3 a(verts[i0], verts[i0 + 1], verts[i0 + 2]);
			vec3 b(verts[i1], verts[i1 + 1], verts[i1 + 2]);
			vec3 c(verts[i2], verts[i2 + 1], verts[i2 + 2]);

			vec3 norm(0, 0, 0);
			norm = glm::cross(b - a, c - a);
			
			normals[i0] += norm.x;
			normals[i0 + 1] += norm.y;
			normals[i0 + 2] += norm.z;

			normals[i1] += norm.x;
			normals[i1 + 1] += norm.y;
			normals[i1 + 2] += norm.z;

			normals[i2] += norm.x;
			normals[i2 + 1] += norm.y;
			normals[i2 + 2] += norm.z;

		}

		//Normalize
		for (unsigned int i = 0; i < normals.size(); i += 3)
		{
			vec3 norm = glm::normalize(vec3(normals[i], normals[i + 1], normals[i + 2]));
			normals[i] = norm.x;
			normals[i + 1] = norm.y;
			normals[i + 2] = norm.z;
			
		}	
		
	}

	void BuildUVs() 
	{
		//Reset uvs
		uvs.clear();
		uvs.resize(verts.size()*2/3, 0);

		unsigned int uv_i = 0;
		
		for (unsigned int i = 0; i < verts.size(); i+=3)
		{
			float x = verts[i];
			float y = verts[i + 1];
			float z = verts[i + 2];

			float xn = normals[i];
			float yn = normals[i + 1];
			float zn = normals[i + 2];

			uvs[uv_i] = x;
			uvs[uv_i + 1] = z;

			if (yn == 0)
			{
				if (zn != 0)
				{
					uvs[uv_i] = x;
					uvs[uv_i + 1] = y;
				}
				if (xn != 0)
				{
					uvs[uv_i] = z;
					uvs[uv_i + 1] = y;
				}
			}

			uv_i += 2;
		}
	}

	void Flip()
	{
		reverse(indices.begin(), indices.end());

		for (unsigned int i = 0; i < texture_ids.size(); i++)
		{
			vec3 position;
			unsigned int id = texture_ids[i];

			switch (id)
			{
			case 0:
				position = vec3(0, 0, 0);
				break;
			case 1:
				position = vec3(1, 0, 0);
				break;
			case 2:
				position = vec3(1, 0, 1);
				break;
			case 3:
				position = vec3(0, 0, 1);
				break;
			case 4:
				position = vec3(0, 1, 0);
				break;
			case 5:
				position = vec3(1, 1, 0);
				break;
			case 6:
				position = vec3(1, 1, 1);
				break;
			case 7:
				position = vec3(0, 1, 1);
				break;
			};

			vec3 direction(
				verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2]);

			direction -= position;
			position += normalize(direction);
			
			if (position.x == 0 && position.y == 0 && position.z == 0)
				texture_ids[i] = 0;
			else if (position.x == 1 && position.y == 0 && position.z == 0)
				texture_ids[i] = 1;
			else if (position.x == 1 && position.y == 0 && position.z == 1)
				texture_ids[i] = 2;
			else if (position.x == 0 && position.y == 0 && position.z == 1)
				texture_ids[i] = 3;
			else if (position.x == 0 && position.y == 1 && position.z == 0)
				texture_ids[i] = 4;
			else if (position.x == 1 && position.y == 1 && position.z == 0)
				texture_ids[i] = 5;
			else if (position.x == 1 && position.y == 1 && position.z == 1)
				texture_ids[i] = 6;
			else if (position.x == 0 && position.y == 1 && position.z == 1)
				texture_ids[i] = 7;
		}
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
		

		for (unsigned int i = 0; i < texture_ids.size(); i++)
		{
			unsigned int x_track = x;
			unsigned int y_track = y;
			unsigned int z_track = z;

			while (x_track != 0)
			{
				switch (texture_ids[i])
				{
				case 0:
					texture_ids[i] = 4;
					break;
				case 1:
					texture_ids[i] = 5;
					break;
				case 2:
					texture_ids[i] = 1;
					break;
				case 3:
					texture_ids[i] = 0;
					break;
				case 4:
					texture_ids[i] = 7;
					break;
				case 5:
					texture_ids[i] = 6;
					break;
				case 6:
					texture_ids[i] = 2;
					break;
				case 7:
					texture_ids[i] = 3;
					break;
				};
				x_track--;
			}

			while (y_track != 0)
			{
				switch (texture_ids[i])
				{
				case 0:
					texture_ids[i] = 3;
					break;
				case 1:
					texture_ids[i] = 0;
					break;
				case 2:
					texture_ids[i] = 1;
					break;
				case 3:
					texture_ids[i] = 2;
					break;
				case 4:
					texture_ids[i] = 7;
					break;
				case 5:
					texture_ids[i] = 4;
					break;
				case 6:
					texture_ids[i] = 5;
					break;
				case 7:
					texture_ids[i] = 6;
					break;
				};
				y_track--;
			}

			while (z_track != 0)
			{
				switch (texture_ids[i])
				{
				case 0:
					texture_ids[i] = 4;
					break;
				case 1:
					texture_ids[i] = 0;
					break;
				case 2:
					texture_ids[i] = 3;
					break;
				case 3:
					texture_ids[i] = 7;
					break;
				case 4:
					texture_ids[i] = 5;
					break;
				case 5:
					texture_ids[i] = 1;
					break;
				case 6:
					texture_ids[i] = 2;
					break;
				case 7:
					texture_ids[i] = 6;
					break;
				};
				z_track--;
			}
		}
	}
};