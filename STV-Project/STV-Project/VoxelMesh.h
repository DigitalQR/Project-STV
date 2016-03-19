#pragma once
#include "MemoryListener.h"
#include "BlockGlossary.h"
#include "Model.h"
#include "ModelLoader.h"

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
	vector<vector<vector<block_id>>> _blocks{ BLOCK_AIR };
};

