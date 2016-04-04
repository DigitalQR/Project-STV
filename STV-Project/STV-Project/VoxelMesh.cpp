#include "VoxelMesh.h"
#include "GameManager.h"

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
	if (_element != nullptr)
	{
		delete _texture_model->model;
		delete _texture_model;
		delete _element;
	}
	if (_model_data != nullptr)
		delete _model_data;
}

void VoxelMesh::SetBlockAt(int x, int y, int z, block_id block) 
{
	_blocks[x][y][z] = block;
}

void VoxelMesh::BuildModel()
{
	Model* model = GameManager::getMain()->model_loader->CreateModel(_model_data->verts, _model_data->uvs, _model_data->normals, _model_data->indices);
	_texture_model = new TexturedModel(model, (GLuint)0);
	_element = new Element3D(_texture_model);

	delete _model_data;
	_model_data = nullptr;
}

void VoxelMesh::ConstructModel()
{
	_model_data = new ModelData();

	const Vectori offset = MESH_SIZE * MESH_OFFSET;
	array<array<array<block_id, 2>, 2>, 2> states;

	for (int x = 0; x < MESH_SIZE.x; x++)
		for (int y = 0; y < MESH_SIZE.y; y++)
			for (int z = 0; z < MESH_SIZE.z; z++)
			{
				states[0][0][0] = GetBlockAt(x, y, z);
				states[1][0][0] = GetBlockAt(x + 1, y, z);
				states[0][1][0] = GetBlockAt(x, y + 1, z);
				states[1][1][0] = GetBlockAt(x + 1, y + 1, z);
				states[0][0][1] = GetBlockAt(x, y, z + 1);
				states[1][0][1] = GetBlockAt(x + 1, y, z + 1);
				states[0][1][1] = GetBlockAt(x, y + 1, z + 1);
				states[1][1][1] = GetBlockAt(x + 1, y + 1, z + 1);
				GameManager::getMain()->voxel_builder->BuildFaces(x + offset.x, y + offset.y, z + offset.z, states, *_model_data);
			}
}
