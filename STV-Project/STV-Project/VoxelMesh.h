#pragma once
#include "MemoryListener.h"
#include "BlockGlossary.h"
#include "Model.h"
#include "ModelLoader.h"

#include <array>
#include <vector>
using namespace std;


#define SQRT_3 1.7320500807557


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
			verts[i+1] += b.y;
			verts[i+2] += b.z;
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
			verts[i+1] -= pivot_y;
			verts[i+2] -= pivot_z;
		}

		float temp_x;
		float temp_y;
		float temp_z;
		float temp_xn;
		float temp_yn;
		float temp_zn;
		
		for (unsigned int i = 0; i < verts.size(); i+=3) {
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

				verts[i] = temp_y ;
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



class MarchingCube {

public:
	MarchingCube() 
	{	
		for(vector<float> values : _case_vertices) 
		{
			_case_normals.push_back(values);
			for (float& f : _case_normals[_case_normals.size() - 1])
				f -= 0.5f;

			_case_uvs.push_back(vector<float>{0,0});
		}
		GenerateParts();
	};

	void RotateIndices(unsigned int x, unsigned int y, unsigned int z, bool& v0, bool& v1, bool& v2, bool& v3, bool& v4, bool& v5, bool& v6, bool& v7) 
	{
		bool t0, t1, t2, t3, t4, t5, t6, t7;

		while (x != 0)
		{
			t0 = v0;
			t1 = v1;
			t2 = v2;
			t3 = v3;
			t4 = v4;
			t5 = v5;
			t6 = v6;
			t7 = v7;

			v0 = t3;
			v1 = t2;
			v2 = t6;
			v3 = t7;
			v4 = t0;
			v5 = t1;
			v6 = t5;
			v7 = t4;

			x--;
		}
		while (y != 0)
		{
			t0 = v0;
			t1 = v1;
			t2 = v2;
			t3 = v3;
			t4 = v4;
			t5 = v5;
			t6 = v6;
			t7 = v7;

			v0 = t1;
			v1 = t2;
			v2 = t3;
			v3 = t0;
			v4 = t5;
			v5 = t6;
			v6 = t7;
			v7 = t4;

			y--;
		}
		while (z != 0)
		{
			t0 = v0;
			t1 = v1;
			t2 = v2;
			t3 = v3;
			t4 = v4;
			t5 = v5;
			t6 = v6;
			t7 = v7;

			v0 = t1;
			v1 = t5;
			v2 = t6;
			v3 = t2;
			v4 = t0;
			v5 = t4;
			v6 = t7;
			v7 = t3;
			z--;
		}
	}

	void GenerateParts();
	void BuildFaces(int x, int y, int z, array<array<array<block_id, 2>, 2>, 2>& states, ModelData& model_data);

protected:
	void SetData(ModelData& model_data, bool v0, bool v1, bool v2, bool v3, bool v4, bool v5, bool v6, bool v7);
	void RotateAndSetData(ModelData& model_data, unsigned int x, unsigned int y, unsigned int z, bool v0, bool v1, bool v2, bool v3, bool v4, bool v5, bool v6, bool v7);
	void SetAllData(unsigned int state_case, bool v0, bool v1, bool v2, bool v3, bool v4, bool v5, bool v6, bool v7);

private:
#define e0	0.5f,0.0f,0.0f
#define e1	1.0f,0.0f,0.5f
#define e2	0.5f,0.0f,1.0f
#define e3	0.0f,0.0f,0.5f
#define e4	0.0f,0.5f,0.0f
#define e5	1.0f,0.5f,0.0f
#define e6	1.0f,0.5f,1.0f
#define e7	0.0f,0.5f,1.0f
#define e8	0.5f,1.0f,0.0f
#define e9	1.0f,1.0f,0.5f
#define e10 0.5f,1.0f,1.0f
#define e11 0.0f,1.0f,0.5f
	//Cases appear in order as seen by http://users.polytech.unice.fr/~lingrand/MarchingCubes/resources/MCcases.gif
	//14 Cases: +ve heavy, where cube is defined from (0,0,0) to (1,1,1)
	//                 v7__________e10_________v6 (1,1,1)
	//                  /|                    /|
	//              e11/ |                e9 / |
	//                /  |                  /  |
	//               /___|______e8_________/   |
	//    (0,1,0) v4|    |e7               |v5 |e6
	//              |    |                 |   |
	//              |    |                 |   |
	//              |    |                 |e5 |
	//              |e4  |__________e2_____|___|
	//              |   / v3 (0,0,1)       |   /v2
	//              |  /e3                 |  /
	//              | /                    | /e1
	//              |/__________e0_________|/
	//              v0 (0,0,0)              v1 (1,0,0)

	const vector<vector<float>> _case_vertices
	{
		{},//None
		{ e0,e4,e3 },//v0 --1
		{ e1,e5,e3,e4 },//v0,v1 --2
		{ e0,e4,e3, e5,e9,e8 },//v0,v5 --3
		{ e0,e4,e3, e9,e10,e6 },//v0,v6 --4
		{ e0,e3,e5,e7,e6 },//v3,v1,v2 --5
		{ e1,e5,e3,e4, e9,e10,e6 },//v0,v1,v6 --6
		{ e1,e5,e0, e8,e11,e4, e9,e10,e6 },//v1,v4,v6 --7
		{ e4,e5,e6,e7 },//v0,v1,v2,v3 --8
		{ e0,e1,e4,e6,e11,e10 },//v0,v2,v3,v7 --9
		{ e0,e3,e8,e11, e2,e1,e9,e10 },//v0,v2,v4,v6 --10
		{ e0,e4,e1,e7,e10,e9 },//v0,v3,v2,v6 --11
		{ e0,e3,e5,e7,e6, e11,e8,e4 },//v1,v2,v3,v4 --12
		{ e0,e4,e3, e5,e9,e8, e1,e2,e6, e11,e10,e7 },//v0,v2,v7,v5 --13
		{ e0,e3,e11,e5,e6,e10 },//v1,v2,v3,v7 --14
	};

	vector<vector<float>> _case_normals;
	vector<vector<float>> _case_uvs;

	const vector<vector<unsigned int>> _case_indices
	{
		{},//None
		{ 0,1,2 },//v0
		{ 1,3,2, 1,2,0 },//v0,v1
		{ 0,1,2,  3,4,5 },//v0,v5
		{ 0,1,2,  5,4,3 },//v0,v6
		{ 0,1,3, 0,3,2, 2,3,4 },//v3,v1,v2
		{ 1,3,2, 1,2,0,  6,5,4 },//v0,v1,v6
		{ 0,1,2,  3,4,5,  6,7,8 },//v1,v4,v6
		{ 3,2,1, 3,1,0 },//v0,v1,v2,v3
		{ 0,3,1, 0,5,3, 2,5,0, 2,4,5 },//v0,v2,v3,v7
		{ 0,2,1, 2,3,1,  4,7,6, 4,6,5 },//v0,v2,v4,v6
		{ 0,1,3, 0,3,5, 0,2,5, 3,5,4 },//v0,v3,v2,v6
		{ 0,1,3, 0,3,2, 2,3,4,  6,5,7 },//v1,v2,v3,v4
		{ 0,1,2,  3,4,5,  6,8,7,  9,10,11 },//v0,v2,v7,v5
		{ 0,1,2, 0,2,4, 0,4,3, 2,5,4 },//v1,v2,v3,v7
	};

	array<array<array<array<array<array<array<array<ModelData, 2>, 2>, 2>, 2>, 2>, 2>, 2>, 2> _model_parts;
};


class VoxelMesh
{
public:
	const Vectori MESH_SIZE;
	const Vectori MESH_OFFSET;

	VoxelMesh(Vectori& mesh_size, Vectori& mesh_offset);
	~VoxelMesh();

	virtual void BuildModel();

	virtual void SetBlockAt(int x, int y, int z, block_id block);

	virtual block_id GetBlockAt(int x, int y, int z)
	{
		return _blocks[x][y][z];
	}

	Model* GetTerrainModel() { return _model; }

protected:
	void AddToPanel(int x, int y, int z, int x_point, int y_point, int z_point,
		vector<float>& verts, vector<float>& uvs, vector<float>& normals, vector<unsigned int>& indices, unsigned int& index_track);

private:
	Model* _model;
	MarchingCube marching_cube_handler;
	vector<vector<vector<block_id>>> _blocks{ BLOCK_AIR };
};
