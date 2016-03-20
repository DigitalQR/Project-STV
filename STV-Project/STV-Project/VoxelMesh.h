#pragma once
#include "MemoryListener.h"
#include "BlockGlossary.h"
#include "Model.h"
#include "ModelLoader.h"
#include "TerrainTools.h"

#include <array>
#include <vector>
using namespace std;


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

private:
	Model* _model;
	vector<vector<vector<block_id>>> _blocks{ BLOCK_AIR };
};
