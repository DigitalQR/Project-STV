#include "VoxelMesh.h"
#include "GameManager.h"
#include "MarchingCube.h"

VoxelMesh::VoxelMesh(Vectori& mesh_size, Vectori& mesh_offset) :
	MESH_SIZE(mesh_size), MESH_OFFSET(mesh_offset)
{
	//Reserve space for chunk
	_blocks.resize(MESH_SIZE.x);
	for (int x = 0; x < MESH_SIZE.x; x++)
	{
		_blocks[x].resize(MESH_SIZE.y);
		for (int y = 0; y < MESH_SIZE.y; y++)
			_blocks[x][y].resize(MESH_SIZE.z);
		
	}
}

VoxelMesh::~VoxelMesh()
{
	delete _model;
}

void VoxelMesh::SetBlockAt(int x, int y, int z, block_id block) 
{
	_blocks[x][y][z] = block;
}

void VoxelMesh::BuildModel()
{
	ModelData model_data;
		
	const Vectori offset = MESH_SIZE * MESH_OFFSET;
	array<array<array<block_id, 2>, 2>, 2> states;

	for (int x = 0; x < MESH_SIZE.x; x++)
		for (int y = 0; y < MESH_SIZE.y; y++)
			for (int z = 0; z < MESH_SIZE.z; z++)
			{
				states[0][0][0] = GetBlockAt(x, y, z);
				states[1][0][0] = GetBlockAt(x+1, y, z);
				states[0][1][0] = GetBlockAt(x, y+1, z);
				states[1][1][0] = GetBlockAt(x+1, y+1, z);
				states[0][0][1] = GetBlockAt(x, y, z+1);
				states[1][0][1] = GetBlockAt(x+1, y, z+1);
				states[0][1][1] = GetBlockAt(x, y+1, z+1);
				states[1][1][1] = GetBlockAt(x+1, y+1, z+1);
				MarchingCube::getMain()->BuildFaces(x + offset.x, y + offset.y, z + offset.z, states, model_data);
			}

	_model = GameManager::getMain()->model_loader->CreateModel(model_data.verts, model_data.uvs, model_data.normals, model_data.indices);
}
