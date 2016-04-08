#pragma once
#include "MemoryListener.h"
#include "BlockGlossary.h"
#include "TexturedModel.h"
#include "ModelLoader.h"
#include "TerrainTools.h"
#include "Element3D.h"

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
	virtual void ConstructModel();

	virtual void SetBlockAt(int x, int y, int z, block_id block);

	virtual block_id GetBlockAt(int x, int y, int z)
	{
		return _blocks[x][y][z];
	}

	Element3D* GetElement() { return _element; };

	bool IsEmptyFlagSet() 
	{
		return full_gen_track == 0 || full_gen_track == MESH_SIZE.x * MESH_SIZE.y * MESH_SIZE.z;
	}

protected:
	unsigned int full_gen_track = 0;

private:
	ModelData* _model_data = nullptr;
	TexturedModel* _texture_model = nullptr;
	Element3D* _element = nullptr;
	vector<vector<vector<block_id>>> _blocks{ BLOCK_AIR };
};
